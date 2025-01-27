
// gcc drm_box.c -o drm_box -I /usr/include/cairo -I /usr/include/drm -ldrm -lcairo
// gst-launch-1.0 -v udpsrc port=5600 caps="application/x-rtp,media=(string)video,encoding-name=(string)H265" \
//      ! rtpjitterbuffer latency=0 ! rtph265depay \
//      ! h265parse ! mppvideodec ! video/x-raw,width=1920,height=1080 ! queue max-size-buffers=1 leaky=downstream \
//      ! kmssink connector-id=111 plane-id=76 fullscreen=true sync=false can-scale=true force-aspect-ratio=true

#include <cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <drm_fourcc.h>

#define BOX_SIZE 200
#define SURFACE_WIDTH 1024
#define SURFACE_HEIGHT 768

struct drm_dev {
    int fd;
    uint32_t conn_id;
    drmModeModeInfo mode;
    uint32_t crtc_id;
    struct drm_dev *next;
};

static int setup_drm(struct drm_dev *dev) {
    printf("Opening DRM device...\n");

    int ret;
    int fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        perror("Cannot open DRM device");
        return -1;
    }

	ret = drmSetClientCap(fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);
	if (ret) {
		fprintf(stderr, "failed to set universal planes cap, %d\n", ret);
		return -1;
	}

	// ret = drmSetClientCap(fd, DRM_CLIENT_CAP_ATOMIC, 1);
	// if (ret) {
	// 	fprintf(stderr, "failed to set atomic cap, %d", ret);
	// 	return -1;
	// }

	// uint64_t cap;
	// if (drmGetCap(fd, DRM_CAP_DUMB_BUFFER, &cap) < 0 || !cap) {
	// 	fprintf(stderr, "drm device '/dev/dri/card0' does not support dumb buffers\n");
	// 	close(fd);
	// 	return -1;
	// }

	// if (drmGetCap(fd, DRM_CAP_CRTC_IN_VBLANK_EVENT, &cap) < 0 || !cap) {
	// 	fprintf(stderr, "drm device '/dev/dri/card0' does not support atomic KMS\n");
	// 	close(fd);
	// 	return -1;
	// }


    dev->fd = fd;
    printf("DRM device opened successfully, fd: %d\n", fd);

    drmModeRes *res = drmModeGetResources(fd);
    if (!res) {
        perror("Cannot get DRM resources");
        close(fd);
        return -1;
    }
    printf("Found %d connectors, %d crtcs\n", res->count_connectors, res->count_crtcs);

    // Get the first available connector
    int found_connector = 0;
    for (int i = 0; i < res->count_connectors; i++) {
        drmModeConnector *conn = drmModeGetConnector(fd, res->connectors[i]);
        if (conn && conn->connection == DRM_MODE_CONNECTED) {
            dev->conn_id = conn->connector_id;
            dev->mode = conn->modes[0];
            printf("Found connected connector %d, mode: %dx%d\n", 
                   conn->connector_id, conn->modes[0].hdisplay, conn->modes[0].vdisplay);
            found_connector = 1;
            drmModeFreeConnector(conn);
            break;
        }
        if (conn) drmModeFreeConnector(conn);
    }

    if (!found_connector) {
        fprintf(stderr, "No connected connector found!\n");
        drmModeFreeResources(res);
        return -1;
    }

    // Get CRTC
    int found_crtc = 0;
    for (int i = 0; i < res->count_crtcs; i++) {
        dev->crtc_id = res->crtcs[i];
        printf("Using CRTC %d\n", res->crtcs[i]);
        found_crtc = 1;
        break;
    }

    if (!found_crtc) {
        fprintf(stderr, "No CRTC found!\n");
        drmModeFreeResources(res);
        return -1;
    }

    // Get all planes
    drmModePlaneRes *plane_res = drmModeGetPlaneResources(fd);
    if (!plane_res) {
        fprintf(stderr, "No plane resources found!\n");
        drmModeFreeResources(res);
        return -1;
    }

    printf("Found %d planes\n", plane_res->count_planes);
    
    // Look for the first available plane that is compatible with the CRTC
    int plane_found = 0;
    uint32_t plane_id = 0;
    for (uint32_t i = 0; i < plane_res->count_planes; i++) {
        drmModePlane *plane = drmModeGetPlane(fd, plane_res->planes[i]);
        if (plane) {

            if (plane->plane_id == 76 || plane->plane_id == 90) {
                printf("Skipping plane 76\n");

                drmModeFreePlane(plane);
                continue;
            }
            
            printf("Checking plane %d (possible_crtcs: 0x%x)\n", 
                   plane->plane_id, plane->possible_crtcs);
            
            // Check if this plane can be used with the current CRTC
            if (plane->possible_crtcs & (1 << 0)) {
                plane_id = plane->plane_id;
                plane_found = 1;
                printf("Found usable plane %d\n", plane_id);

                drmModeObjectProperties *props = drmModeObjectGetProperties(fd, plane_id, DRM_MODE_OBJECT_PLANE);
                if (!props) {
                    fprintf(stderr, "Cannot get properties for plane %d\n", plane_id);
                    return -1;
                }

                uint32_t zpos_prop_id = 0;

                // Iterate through properties to find "zpos"
                for (uint32_t j = 0; j < props->count_props; j++) {
                    drmModePropertyRes *prop = drmModeGetProperty(fd, props->props[j]);
                    if (!prop) {
                        continue;
                    }

                    if (strcmp(prop->name, "zpos") == 0) {
                        zpos_prop_id = prop->prop_id;
                        drmModeFreeProperty(prop);
                        break;
                    }
                    drmModeFreeProperty(prop);
                }

                if (zpos_prop_id) {
                    // Set zpos to a high value (e.g., 100000)
                    int ret = drmModeObjectSetProperty(fd, plane_id, DRM_MODE_OBJECT_PLANE, zpos_prop_id, 2);
                    if (ret < 0) {
                        perror("Failed to set zpos");
                        drmModeFreeObjectProperties(props);
                        return -1;
                    }
                    printf("zpos set to topmost value for plane %d\n", plane_id);
                } else {
                    printf("zpos property not found for plane %d\n", plane_id);
                }

                drmModeFreeObjectProperties(props);





                drmModeFreePlane(plane);
                break;
            }
            drmModeFreePlane(plane);
        }
    }

    if (!plane_found) {
        fprintf(stderr, "No compatible planes found!\n");
        drmModeFreeResources(res);
        drmModeFreePlaneResources(plane_res);
        return -1;
    }



                drmModeObjectProperties *props = drmModeObjectGetProperties(fd, 76, DRM_MODE_OBJECT_PLANE);
                if (!props) {
                    fprintf(stderr, "Cannot get properties for plane %d\n", plane_id);
                    return -1;
                }

                // Iterate through properties to find "zpos"
                for (uint32_t j = 0; j < props->count_props; j++) {
                    drmModePropertyRes *prop = drmModeGetProperty(fd, props->props[j]);
                    if (!prop) {
                        continue;
                    }

                    if (strcmp(prop->name, "zpos") == 0) {
                        printf("zpos: %i\n",prop->prop_id);
                        drmModeFreeProperty(prop);
                        break;
                    }
                    drmModeFreeProperty(prop);
                }





                props = drmModeObjectGetProperties(fd, 76, DRM_MODE_OBJECT_PLANE);
                if (!props) {
                    fprintf(stderr, "Cannot get properties for plane %d\n", plane_id);
                    return -1;
                }

                uint32_t zpos_prop_id = 0;

                // Iterate through properties to find "zpos"
                for (uint32_t j = 0; j < props->count_props; j++) {
                    drmModePropertyRes *prop = drmModeGetProperty(fd, props->props[j]);
                    if (!prop) {
                        continue;
                    }

                    if (strcmp(prop->name, "zpos") == 0) {
                        zpos_prop_id = prop->prop_id;
                        drmModeFreeProperty(prop);
                        break;
                    }
                    drmModeFreeProperty(prop);
                }

                if (zpos_prop_id) {
                    // Set zpos to a high value (e.g., 100000)
                    int ret = drmModeObjectSetProperty(fd, 76, DRM_MODE_OBJECT_PLANE, zpos_prop_id, 0);
                    if (ret < 0) {
                        perror("Failed to set zpos");
                        drmModeFreeObjectProperties(props);
                        return -1;
                    }
                    printf("zpos set to topmost value for plane %d\n", plane_id);
                } else {
                    printf("zpos property not found for plane %d\n", plane_id);
                }



                props = drmModeObjectGetProperties(fd, 76, DRM_MODE_OBJECT_PLANE);
                if (!props) {
                    fprintf(stderr, "Cannot get properties for plane %d\n", plane_id);
                    return -1;
                }

                // Iterate through properties to find "zpos"
                for (uint32_t j = 0; j < props->count_props; j++) {
                    drmModePropertyRes *prop = drmModeGetProperty(fd, props->props[j]);
                    if (!prop) {
                        continue;
                    }

                    if (strcmp(prop->name, "zpos") == 0) {
                        printf("zpos: %i\n",prop->prop_id);
                        drmModeFreeProperty(prop);
                        break;
                    }
                    drmModeFreeProperty(prop);
                }




    drmModeFreePlaneResources(plane_res);
    drmModeFreeResources(res);
    return plane_id;
}

int main() {
    struct drm_dev dev = {0};
    uint32_t plane_id;

    // Setup DRM and get a plane
    plane_id = setup_drm(&dev);
    if (plane_id == (uint32_t)-1) {
        fprintf(stderr, "Failed to setup DRM\n");
        return -1;
    }

    printf("Creating Cairo surface...\n");
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 
                                                        SURFACE_WIDTH, 
                                                        SURFACE_HEIGHT);
    cairo_t *cr = cairo_create(surface);

    // Clear background to white for better visibility
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    // Draw red box for better visibility
    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
    cairo_rectangle(cr, 
                   (SURFACE_WIDTH - BOX_SIZE) / 2, 
                   (SURFACE_HEIGHT - BOX_SIZE) / 2, 
                   BOX_SIZE, 
                   BOX_SIZE);
    cairo_fill(cr);

    printf("Creating DRM framebuffer...\n");
    uint32_t fb_id;
    uint32_t handle;
    struct drm_mode_create_dumb create = {
        .width = SURFACE_WIDTH,
        .height = SURFACE_HEIGHT,
        .bpp = 32
    };

    if (drmIoctl(dev.fd, DRM_IOCTL_MODE_CREATE_DUMB, &create) < 0) {
        perror("Cannot create dumb buffer");
        goto cleanup;
    }
    printf("Dumb buffer created, size: %llu\n", create.size);

    handle = create.handle;

    struct drm_mode_map_dumb map = {
        .handle = handle
    };

    if (drmIoctl(dev.fd, DRM_IOCTL_MODE_MAP_DUMB, &map) < 0) {
        perror("Cannot map dumb buffer");
        goto cleanup;
    }
    printf("Buffer mapped at offset: %llu\n", map.offset);

    uint8_t *fb_data = mmap(0, create.size, PROT_READ | PROT_WRITE, MAP_SHARED,
                           dev.fd, map.offset);
    if (fb_data == MAP_FAILED) {
        perror("Cannot mmap framebuffer");
        goto cleanup;
    }
    printf("Framebuffer mmap successful\n");

    printf("Copying Cairo surface to framebuffer...\n");
    memcpy(fb_data, cairo_image_surface_get_data(surface),
           SURFACE_WIDTH * SURFACE_HEIGHT * 4);

    printf("Creating framebuffer object...\n");
    if (drmModeAddFB(dev.fd, SURFACE_WIDTH, SURFACE_HEIGHT, 24, 32, 
                     SURFACE_WIDTH * 4, handle, &fb_id)) {
        perror("Cannot create framebuffer");
        goto cleanup;
    }
    printf("Framebuffer created with ID: %d\n", fb_id);

    printf("Setting up plane...\n");
    int ret = drmModeSetPlane(dev.fd, plane_id, dev.crtc_id, fb_id, 0,
                             0, 0, SURFACE_WIDTH, SURFACE_HEIGHT,
                             0, 0, SURFACE_WIDTH << 16, SURFACE_HEIGHT << 16);
    if (ret < 0) {
        perror("Failed to set plane");
        goto cleanup;
    }
    printf("Plane set successfully\n");

    printf("Displaying for 50 seconds...\n");
    sleep(50);

cleanup:
    printf("Cleaning up...\n");
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    close(dev.fd);
    return 0;
}
