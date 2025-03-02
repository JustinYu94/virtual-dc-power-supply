#ifndef _VDC_CORE_H
#define _VDC_CORE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define VDC_MAX_HANDLE          16

typedef int32_t         vdc_result;

#define VDC_SUCCESS                                 0
#define VDC_ERROR_INTERNAL                          1L
#define VDC_ERROR_MAX_HANDLE                        2L
#define VDC_ERROR_INVALID_HANDLE                    3L
#define VDC_ERROR_NOT_ENOUGH_BUFFER                 4L
#define VDC_ERROR_PARAM_INVALID                     5L
#define VDC_ERROR_PARAM_NULL                        6L

typedef uint32_t         vdc_handle;

typedef struct vdc_param_spec {
    float min;
    float max;
    float resolution;
} vdc_param_spec;

typedef struct vdc_status {
    float output_voltage;
    float output_current;
    float output_power;
    bool output_state;
} vdc_status;

typedef struct vdc_supplied {
    float voltage;
    float current;
} vdc_supplied;

typedef void (*vdc_supplied_callback)(const vdc_supplied *supply, vdc_supplied *response, void *user_data);

typedef struct vdc_uut {
    vdc_supplied_callback uut_response;
    void *user_data;
} vdc_uut;

vdc_result vdc_create(vdc_handle *handle);
vdc_result vdc_destroy(vdc_handle handle);

vdc_result vdc_get_model(vdc_handle handle, char* model, size_t model_size);
vdc_result vdc_get_voltage_spec(vdc_handle handle, vdc_param_spec *spec);
vdc_result vdc_get_current_spec(vdc_handle handle, vdc_param_spec *spec);

vdc_result vdc_set_voltage(vdc_handle handle, float value);
vdc_result vdc_get_voltage(vdc_handle handle, float *value);
vdc_result vdc_set_current(vdc_handle handle, float value);
vdc_result vdc_get_current(vdc_handle handle, float *value);
vdc_result vdc_set_output_state(vdc_handle handle, bool value);

vdc_result vdc_connect_uut(vdc_handle handle, const vdc_uut *uut);
vdc_result vdc_disconnect_uut(vdc_handle handle, const vdc_uut *uut);

vdc_result vdc_read_status(vdc_handle handle, vdc_status *status);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _VDC_CORE_H