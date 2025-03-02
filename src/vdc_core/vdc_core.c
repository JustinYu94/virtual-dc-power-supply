#include "vdc_core.h"

#define VDC_MODEL_SIZE         32

typedef struct virtual_dc_power_supply {
    float voltage;
    float current;
    float min_voltage;
    float max_voltage;
    float min_current;
    float max_current;
    bool output_state;
    char model[VDC_MODEL_SIZE];
} virtual_dc_power_supply;

typedef enum vdc_slot_state {
    VDC_SLOT_STATE_FREE,
    VDC_SLOT_STATE_CREATED,
} vdc_slot_state;

typedef struct vdc_slot {
    virtual_dc_power_supply vdc;
    vdc_slot_state state;
} vdc_slot;

static vdc_slot _vdc_pool[VDC_MAX_HANDLE];

static int _vdc_search_free_slot()
{
    return -1;
}

vdc_result vdc_create(vdc_handle *handle)
{
    if (!handle)
    {
        return VDC_ERROR_PARAM_NULL;
    }

    int free_slot_index = _vdc_search_free_slot();
    if (free_slot_index == -1)
    {
        return VDC_ERROR_MAX_HANDLE;
    }

    return VDC_ERROR_INTERNAL;
}

vdc_result vdc_destroy(vdc_handle handle)
{
    return VDC_ERROR_INTERNAL;
}

vdc_result vdc_get_model(vdc_handle handle, char* model, size_t model_size)
{
    return VDC_ERROR_INTERNAL;
}

vdc_result vdc_get_voltage_spec(vdc_handle handle, vdc_param_spec *spec)
{
    return VDC_ERROR_INTERNAL;
}

vdc_result vdc_get_current_spec(vdc_handle handle, vdc_param_spec *spec)
{
    return VDC_ERROR_INTERNAL;
}

vdc_result vdc_set_voltage(vdc_handle handle, float value)
{
    return VDC_ERROR_INTERNAL;
}

vdc_result vdc_get_voltage(vdc_handle handle, float *value)
{
    return VDC_ERROR_INTERNAL;
}

vdc_result vdc_set_current(vdc_handle handle, float value)
{
    return VDC_ERROR_INTERNAL;
}

vdc_result vdc_get_current(vdc_handle handle, float *value)
{
    return VDC_ERROR_INTERNAL;
}

vdc_result vdc_set_output_state(vdc_handle handle, bool value)
{
    return VDC_ERROR_INTERNAL;
}

vdc_result vdc_connect_uut(vdc_handle handle, const vdc_uut *uut)
{
    return VDC_ERROR_INTERNAL;
}

vdc_result vdc_disconnect_uut(vdc_handle handle, const vdc_uut *uut)
{
    return VDC_ERROR_INTERNAL;
}

vdc_result vdc_read_status(vdc_handle handle, vdc_status *status)
{
    return VDC_ERROR_INTERNAL;
}