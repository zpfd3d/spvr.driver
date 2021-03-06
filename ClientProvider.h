/*
 * Copyright (c) 2016
 *  Somebody
 */
#ifndef SPVR_CLIENTPROVIDER_H
#define SPVR_CLIENTPROVIDER_H

#include "openvr_driver.h"

#include <cstdint>

namespace spvr
{

class Logger;

class SmartClient final : public vr::IClientTrackedDeviceProvider
{
    Logger *m_pLogger;
public:
    SmartClient();
    /** initializes the driver. This will be called before any other methods are called,
    * except BIsHmdPresent(). BIsHmdPresent is called outside of the Init/Cleanup pair.
    * If Init returns anything other than VRInitError_None the driver DLL will be unloaded.
    *
    * pDriverHost will never be NULL, and will always be a pointer to a IClientDriverHost interface
    *
    * pchUserDriverConfigDir - The absolute path of the directory where the driver should store user
    *	config files.
    * pchDriverInstallDir - The absolute path of the root directory for the driver.
    */
    virtual vr::EVRInitError Init(vr::IDriverLog *pDriverLog, vr::IClientDriverHost *pDriverHost, char const *pchUserDriverConfigDir, char const *pchDriverInstallDir) override;

    /** cleans up the driver right before it is unloaded */
    virtual void Cleanup() override;

    /** Called when the client needs to inform an application if an HMD is attached that uses
    * this driver. This method should be as lightweight as possible and should have no side effects
    * such as hooking process functions or leaving resources loaded. Init will not be called before
    * this method and Cleanup will not be called after it.
    */
    virtual bool BIsHmdPresent(char const *pchUserConfigDir) override;

    /** called when the client inits an HMD to let the client driver know which one is in use */
    virtual vr::EVRInitError SetDisplayId(char const *pchDisplayId) override;

    /** Returns the stencil mesh information for the current HMD. If this HMD does not have a stencil mesh the vertex data and count will be
    * NULL and 0 respectively. This mesh is meant to be rendered into the stencil buffer (or into the depth buffer setting nearz) before rendering
    * each eye's view. The pixels covered by this mesh will never be seen by the user after the lens distortion is applied and based on visibility to the panels.
    * This will improve perf by letting the GPU early-reject pixels the user will never see before running the pixel shader.
    * NOTE: Render this mesh with backface culling disabled since the winding order of the vertices can be different per-HMD or per-eye.
    */
    virtual vr::HiddenAreaMesh_t GetHiddenAreaMesh(vr::EVREye eEye) override;

    /** Get the MC image for the current HMD.
    * Returns the size in bytes of the buffer required to hold the specified resource. */
    virtual std::uint32_t GetMCImage(std::uint32_t *pImgWidth, std::uint32_t *pImgHeight, std::uint32_t *pChannels, void *pDataBuffer, std::uint32_t unBufferLen) override;
};

} // namespace spvr

#endif // SPVR_CLIENTPROVIDER_H
