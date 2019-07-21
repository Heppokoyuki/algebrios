/*
 * Copyright (c) 2015 KANATSU Minoru
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <efi.h>
#include <efilib.h>

EFI_STATUS
efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *systab)
{
    EFI_STATUS status = EFI_SUCCESS;
    EFI_LOADED_IMAGE *loadedImageParent;
    EFI_LOADED_IMAGE *loadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sftp;
    EFI_DEVICE_PATH *path;
    EFI_HANDLE _image;

    /* initalize gnu-efi */
    InitalizeLib(image, systab);
    Print(L"Hello from bootloader!\n");

    status = uefi_call_wrapper(BS->OpenProtocol, 6, image, &LoadedImageProtocol,
                               &LoadedImageParent, image, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if(EFI_ERROR(Status)) {
        Print(L"Could not get LoadedImageProtocol handler %r\n", status);
        return status;
    }

    path = FileDevicePath(LoadedImageParent->DeviceHandle, L"kernel.bin");
    if(path == NULL) {
        Print(L"Could not get device path\n");
        return EFI_INVALID_PARAMETER;
    }

    status = uefi_call_wrapper(BS->LoadImage, 6, false, image, path, NULL, 0, &image);
    if(EFI_ERROR(status)) {
        Print(L"Could not load %r", status);
        FreePool(path);
        return status;
    }

    status = uefi_call_wrapper(BS->OpenProtocol, 6, _image, &LoadedImageProtocol,
                               &LoadedImage, image, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if(EFI_STATUS(status)) {
        Print(L"Could not get LoadedImageProtocol handler %r\n", status);
        uefi_call_wrapper(BS->UnloadImage, 1, _image);
        FreePool(path);
        return status;
    }
    LoadedImage->LoadOptions = Options;
}

