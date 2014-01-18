/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include <xenia/kernel/xam_info.h>

#include <xenia/kernel/kernel_state.h>
#include <xenia/kernel/xam_private.h>
#include <xenia/kernel/util/shim_utils.h>
#include <xenia/kernel/util/xex2.h>


using namespace xe;
using namespace xe::kernel;
using namespace xe::kernel::xam;


namespace xe {
namespace kernel {


SHIM_CALL XamGetSystemVersion_shim(
    PPCContext* ppc_state, KernelState* state) {
  XELOGD("XamGetSystemVersion()");
  // eh, just picking one. If we go too low we may break new games, but
  // this value seems to be used for conditionally loading symbols and if
  // we pretend to be old we have less to worry with implementing.
  // 0x200A3200
  // 0x20096B00
  SHIM_SET_RETURN(0);
}


SHIM_CALL XGetAVPack_shim(
    PPCContext* ppc_state, KernelState* state) {
  // DWORD
  // Not sure what the values are for this, but 6 is VGA.
  // Other likely values are 3/4/8 for HDMI or something.
  // Games seem to use this as a PAL check - if the result is not 3/4/6/8
  // they explode with errors if not in PAL mode.
  SHIM_SET_RETURN(6);
}


SHIM_CALL XGetGameRegion_shim(
    PPCContext* ppc_state, KernelState* state) {
  XELOGD("XGetGameRegion()");

  SHIM_SET_RETURN(XEX_REGION_ALL);
}


SHIM_CALL XGetLanguage_shim(
    PPCContext* ppc_state, KernelState* state) {
  XELOGD("XGetLanguage()");

  uint32_t desired_language = X_LANGUAGE_ENGLISH;

  // Switch the language based on game region.
  // TODO(benvanik): pull from xex header.
  uint32_t game_region = XEX_REGION_NTSCU;
  if (game_region & XEX_REGION_NTSCU) {
    desired_language = X_LANGUAGE_ENGLISH;
  } else if (game_region & XEX_REGION_NTSCJ) {
    desired_language = X_LANGUAGE_JAPANESE;
  }
  // Add more overrides?

  SHIM_SET_RETURN(desired_language);
}


SHIM_CALL XamLoaderGetLaunchDataSize_shim(
    PPCContext* ppc_state, KernelState* state) {
  uint32_t size_ptr = SHIM_GET_ARG_32(0);

  XELOGD(
      "XamLoaderGetLaunchDataSize(%.8X)",
      size_ptr);

  SHIM_SET_MEM_32(size_ptr, 0);

  SHIM_SET_RETURN(0);
}


SHIM_CALL XamLoaderGetLaunchData_shim(
    PPCContext* ppc_state, KernelState* state) {
  uint32_t buffer_ptr = SHIM_GET_ARG_32(0);
  uint32_t buffer_size = SHIM_GET_ARG_32(1);

  XELOGD(
      "XamLoaderGetLaunchData(%.8X, %d)",
      buffer_ptr, buffer_size);

  SHIM_SET_RETURN(0);
}


}  // namespace kernel
}  // namespace xe


void xe::kernel::xam::RegisterInfoExports(
    ExportResolver* export_resolver, KernelState* state) {
  SHIM_SET_MAPPING("xam.xex", XamGetSystemVersion, state);
  SHIM_SET_MAPPING("xam.xex", XGetAVPack, state);
  SHIM_SET_MAPPING("xam.xex", XGetGameRegion, state);
  SHIM_SET_MAPPING("xam.xex", XGetLanguage, state);

  SHIM_SET_MAPPING("xam.xex", XamLoaderGetLaunchDataSize, state);
  SHIM_SET_MAPPING("xam.xex", XamLoaderGetLaunchData, state);
}