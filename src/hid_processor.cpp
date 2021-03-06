// Copyright (c) 2017, Smart Projects Holdings Ltd
// All rights reserved.
// See LICENSE file for license details.

/** HID device driver. */

#ifndef VSM_DISABLE_HID

#include <ugcs/vsm/hid_processor.h>

using namespace ugcs::vsm;

Singleton<Hid_processor> Hid_processor::singleton;

Hid_processor::Hid_processor()
{
}

Hid_processor::Stream::Ref
Hid_processor::Open(uint32_t vendor_id, uint32_t product_id)
{
    Device_id device_id(vendor_id, product_id);

    std::unique_lock<std::mutex> lock(stream_mutex);
    auto it = streams.find(device_id);
    if (it != streams.end()) {
        if (it->second.lock()) {
            VSM_EXCEPTION(Already_opened_exception, "Device is already opened");
        }
    } else {
        it = streams.emplace(device_id, Stream::Weak_ptr()).first;
    }
    auto stream = Create_stream(device_id);
    Register_stream(stream);
    it->second = stream;
    return stream;
}

/* Hid_processor::Stream class. */
Hid_processor::Stream::Stream(Hid_processor::Ptr processor, Device_id device_id,
                Hid_processor::Stream::Native_handle::Ptr native_handle,
                File_processor::Stream::Native_handle::Unique_ptr&& native_file_handle) :
                        File_processor::Stream::Stream(
                                processor,
                                native_handle->file_name,
                                File_processor::Stream::Mode("r+"), false,
                                std::move(native_file_handle)),
                        proc(processor), device_id(device_id),
                        native_handle(native_handle)
{
}

void
Hid_processor::Stream::Set_output_report(Io_buffer::Ptr data, uint8_t report_id)
{
    native_handle->Set_output_report(data, report_id);
}

Io_buffer::Ptr
Hid_processor::Stream::Get_input_report(size_t report_size, uint8_t report_id)
{
    return native_handle->Get_input_report(report_size, report_id);
}

#endif /* VSM_DISABLE_HID */
