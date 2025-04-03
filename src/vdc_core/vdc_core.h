#ifndef _VDC_CORE_H
#define _VDC_CORE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define VDC_MAX_HANDLE          16

typedef int32_t         vdc_result_t;

#define VDC_SUCCESS                                 0
#define VDC_ERROR_INTERNAL                          1L
#define VDC_ERROR_MAX_HANDLE                        2L
#define VDC_ERROR_INVALID_HANDLE                    3L
#define VDC_ERROR_BUFFER_TOO_SMALL                  4L
#define VDC_ERROR_PARAM_INVALID                     5L
#define VDC_ERROR_PARAM_NULL                        6L
#define VDC_ERROR_UUT_CONNECTED                     7L

typedef uint32_t         vdc_handle_t;

typedef struct vdc_param_spec_t {
    float min;
    float max;
    float resolution;
} vdc_param_spec_t;

typedef struct vdc_status_t {
    float output_voltage;
    float output_current;
    float output_power;
    bool output_state;
} vdc_status_t;

typedef struct vdc_output_power_t {
    float voltage;
    float current;
} vdc_output_power_t;

typedef void (*vdc_supply_power_callback_t)(const vdc_output_power_t* supply, vdc_output_power_t* response, void* user_data);

typedef struct vdc_uut_t {
    vdc_supply_power_callback_t supply_power;
    void *user_data;
} vdc_uut_t;

vdc_result_t vdc_create(vdc_handle_t *handle);
vdc_result_t vdc_destroy(vdc_handle_t handle);

vdc_result_t vdc_get_model(vdc_handle_t handle, char* model, size_t* model_size);
vdc_result_t vdc_get_voltage_spec(vdc_handle_t handle, vdc_param_spec_t* spec);
vdc_result_t vdc_get_current_spec(vdc_handle_t handle, vdc_param_spec_t* spec);

vdc_result_t vdc_set_voltage(vdc_handle_t handle, float value);
vdc_result_t vdc_get_voltage(vdc_handle_t handle, float* value);
vdc_result_t vdc_set_current(vdc_handle_t handle, float value);
vdc_result_t vdc_get_current(vdc_handle_t handle, float* value);
vdc_result_t vdc_set_output_state(vdc_handle_t handle, bool value);

vdc_result_t vdc_connect_uut(vdc_handle_t handle, const vdc_uut_t* uut);
vdc_result_t vdc_disconnect_uut(vdc_handle_t handle);
vdc_result_t vdc_is_uut_connected(vdc_handle_t handle, bool* value);

vdc_result_t vdc_read_status(vdc_handle_t handle, vdc_status_t* status);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _VDC_CORE_H