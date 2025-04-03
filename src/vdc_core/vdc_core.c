#include <assert.h>
#include <string.h>
#include "vdc_core.h"

#define VDC_ASSERT(x)       assert(x)

#define VDC_MODEL_SIZE          32
#define VDC_INVALID_SLOT        (-1)
#define VDC_INVALID_HANDLE      0

typedef struct _vdc_t {
    float voltage;
    float current;
    float min_voltage;
    float max_voltage;
    float min_current;
    float max_current;
    bool output_state;
    char* model;
    const vdc_uut_t* uut;
} _vdc_t;

typedef enum _vdc_slot_state {
    VDC_SLOT_STATE_FREE,
    VDC_SLOT_STATE_CREATED,
} _vdc_slot_state;

typedef struct _vdc_slot_t {
    _vdc_t instance;
    _vdc_slot_state state;
} _vdc_slot_t;

static _vdc_slot_t _vdc_pool[VDC_MAX_HANDLE];

static _vdc_slot_t* _vdc_slot_at(vdc_handle_t handle)
{
    if (handle == VDC_INVALID_HANDLE || handle > VDC_MAX_HANDLE)
    {
        return NULL;
    }
    return &_vdc_pool[handle - 1];
}

static _vdc_t* _vdc_lookup_instance(vdc_handle_t handle)
{
    if (handle < 1 || handle > VDC_MAX_HANDLE)
    {
        return NULL;
    }

    _vdc_slot_t* slot = &_vdc_pool[handle - 1];
    if (slot->state == VDC_SLOT_STATE_CREATED)
    {
        return &slot->instance;
    }
    return NULL;
}

static void _vdc_init_instance_model_a(_vdc_t* vdc)
{
    VDC_ASSERT(vdc);

    vdc->min_voltage = 0.0;
    vdc->max_voltage = 15.0;

    vdc->min_current = 0.0;
    vdc->max_current = 4.0;

    vdc->output_state = false;
    vdc->model = "MODE-A";
    vdc->uut = NULL;
}

vdc_result_t vdc_create(vdc_handle_t *handle)
{
    if (!handle)
    {
        return VDC_ERROR_PARAM_NULL;
    }

    int slot_index = -1;
    for (int i = 0; i < VDC_MAX_HANDLE; i++)
    {
        if (_vdc_pool[i].state == VDC_SLOT_STATE_FREE)
        {
            slot_index = i;
            break;;
        }
    }
    if (slot_index == -1)
    {
        return VDC_ERROR_MAX_HANDLE;
    }

    _vdc_slot_t* slot = &_vdc_pool[slot_index];
    slot->state = VDC_SLOT_STATE_CREATED;
    _vdc_t* vdc = &slot->instance;
    _vdc_init_instance_model_a(vdc);
    *handle = slot_index + 1;

    return VDC_SUCCESS;
}

vdc_result_t vdc_destroy(vdc_handle_t handle)
{
    _vdc_slot_t* slot = _vdc_slot_at(handle);
    if (!slot)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }
    slot->state = VDC_SLOT_STATE_FREE;
    return VDC_SUCCESS;
}

vdc_result_t vdc_get_model(vdc_handle_t handle, char* model, size_t* model_size)
{
    _vdc_t* vdc = _vdc_lookup_instance(handle);
    if (!vdc)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }
    if (!model_size)
    {
        return VDC_ERROR_PARAM_NULL;
    }

    size_t required_size = strlen(vdc->model) + 1;
    if (model == NULL)
    {
        *model_size = required_size;
        return VDC_SUCCESS;
    }
    if (*model_size < required_size)
    {
        return VDC_ERROR_BUFFER_TOO_SMALL;
    }

    strncpy_s(model, *model_size, vdc->model, required_size);
    return VDC_SUCCESS;
}

vdc_result_t vdc_get_voltage_spec(vdc_handle_t handle, vdc_param_spec_t *spec)
{
    _vdc_t* vdc = _vdc_lookup_instance(handle);
    if (!vdc)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }
    if (!spec)
    {
        return VDC_ERROR_PARAM_NULL;
    }

    spec->min = vdc->min_voltage;
    spec->max = vdc->max_voltage;
    return VDC_SUCCESS;
}

vdc_result_t vdc_get_current_spec(vdc_handle_t handle, vdc_param_spec_t *spec)
{
    _vdc_t* vdc = _vdc_lookup_instance(handle);
    if (!vdc)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }
    if (!spec)
    {
        return VDC_ERROR_PARAM_NULL;
    }

    spec->min = vdc->min_current;
    spec->max = vdc->max_current;
    return VDC_ERROR_INTERNAL;
}

vdc_result_t vdc_set_voltage(vdc_handle_t handle, float value)
{
    _vdc_t* vdc = _vdc_lookup_instance(handle);
    if (!vdc)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }
    if (value < vdc->min_voltage || value > vdc->max_voltage)
    {
        return VDC_ERROR_PARAM_INVALID;
    }

    vdc->voltage = value;
    return VDC_SUCCESS;
}

vdc_result_t vdc_get_voltage(vdc_handle_t handle, float *value)
{
    _vdc_t* vdc = _vdc_lookup_instance(handle);
    if (!vdc)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }
    if (!value)
    {
        return VDC_ERROR_PARAM_NULL;
    }

    *value = vdc->voltage;
    return VDC_SUCCESS;
}

vdc_result_t vdc_set_current(vdc_handle_t handle, float value)
{
    _vdc_t* vdc = _vdc_lookup_instance(handle);
    if (!vdc)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }
    if (value < vdc->min_current || value > vdc->max_current)
    {
        return VDC_ERROR_PARAM_INVALID;
    }

    vdc->current = value;
    return VDC_SUCCESS;
}

vdc_result_t vdc_get_current(vdc_handle_t handle, float *value)
{
    _vdc_t* vdc = _vdc_lookup_instance(handle);
    if (!vdc)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }
    if (!value)
    {
        return VDC_ERROR_PARAM_NULL;
    }

    *value = vdc->current;
    return VDC_SUCCESS;
}

vdc_result_t vdc_set_output_state(vdc_handle_t handle, bool value)
{
    _vdc_t* vdc = _vdc_lookup_instance(handle);
    if (!vdc)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }

    vdc->output_state = value;
    return VDC_SUCCESS;
}

vdc_result_t vdc_connect_uut(vdc_handle_t handle, const vdc_uut_t *uut)
{
    _vdc_t* vdc = _vdc_lookup_instance(handle);
    if (!vdc)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }
    if (!uut)
    {
        return VDC_ERROR_PARAM_NULL;
    }
    if (vdc->uut)
    {
        return VDC_ERROR_UUT_CONNECTED;
    }

    vdc->uut = uut;
    return VDC_SUCCESS;
}

vdc_result_t vdc_disconnect_uut(vdc_handle_t handle)
{
    _vdc_t* vdc = _vdc_lookup_instance(handle);
    if (!vdc)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }

    vdc->uut = NULL;
    return VDC_SUCCESS;
}

vdc_result_t vdc_is_uut_connected(vdc_handle_t handle, bool* value)
{
    _vdc_t* vdc = _vdc_lookup_instance(handle);
    if (!vdc)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }
    if (!value)
    {
        return VDC_ERROR_PARAM_NULL;
    }

    *value = (vdc->uut != NULL);
    return VDC_SUCCESS;
}

vdc_result_t vdc_read_status(vdc_handle_t handle, vdc_status_t *status)
{
    _vdc_t* vdc = _vdc_lookup_instance(handle);
    if (!vdc)
    {
        return VDC_ERROR_INVALID_HANDLE;
    }
    if (!status)
    {
        return VDC_ERROR_PARAM_NULL;
    }

    status->output_state = vdc->output_state;
    if (vdc->output_state == true)
    {
        if (vdc->uut)
        {
            vdc_output_power_t supply = { 0 };
            vdc_output_power_t response = { 0 };
            vdc->uut->supply_power(&supply, &response, vdc->uut->user_data);

            status->output_voltage = response.voltage;
            status->output_current = response.current;
            status->output_power = response.voltage * response.current;
        }
        else
        {
            status->output_voltage = vdc->voltage;
            status->output_current = 0.0;
            status->output_power = 0.0;
        }
    }
    else
    {
        status->output_voltage = 0.0;
        status->output_current = 0.0;
        status->output_power = 0.0;
    }

    return VDC_SUCCESS;
}