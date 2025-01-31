#include <unistd.h>
#include <libdrm/drm.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "lvgl/lvgl.h"
#include "lvgl/src/core/lv_global.h"

#include "src/ui/ui.h"


#include <xf86drm.h>
#include <xf86drmMode.h>
struct drm_dev {
    int fd;
    uint32_t conn_id;
    drmModeModeInfo mode;
    uint32_t crtc_id;
    struct drm_dev *next;
};

static void lv_linux_disp_init(void)
{
    const char *device = "/dev/dri/card0";
    lv_display_t * disp = lv_linux_drm_create();

    lv_linux_drm_set_file(disp, device, -1);
}
static void btn_event_cb(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    printf("Button pressed!\n");
}

void create_menu(void) {
    lv_obj_t * scr = lv_scr_act();
    lv_obj_t * label;

    lv_obj_t * btn1 = lv_btn_create(scr);
    lv_obj_set_pos(btn1, 100, 100);
    lv_obj_set_size(btn1, 120, 50);
    lv_obj_add_event_cb(btn1, btn_event_cb, LV_EVENT_CLICKED, NULL);
    
    label = lv_label_create(btn1);
    lv_label_set_text(label, "Option 1");
    lv_obj_center(label);

    lv_obj_t * btn2 = lv_btn_create(scr);
    lv_obj_set_pos(btn2, 100, 200);
    lv_obj_set_size(btn2, 120, 50);
    lv_obj_add_event_cb(btn2, btn_event_cb, LV_EVENT_CLICKED, NULL);
    
    label = lv_label_create(btn2);
    lv_label_set_text(label, "Option 2");
    lv_obj_center(label);
}



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


int main(int argc, char **argv)
{

    /* Initialize LVGL. */
    lv_init();

    /* Initialize libDRM */
    lv_linux_disp_init();

    ui_init();

    while (1) {
        lv_task_handler();
        //ui_tick();
        usleep(5000);  // Sleep for a short period to allow LVGL to update
    }

    //lv_linux_run_loop();
    //sleep(50);

    return 0;
}
