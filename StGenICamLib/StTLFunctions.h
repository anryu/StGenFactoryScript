///  \addtogroup DLLInfo   TL Global Info Function



//▼1.0.0.73 GenApi3.0
//このファイルでGenICam::ClientをGenTLに置換 _GENAPI3内
//▲1.0.0.73 GenApi3.0

#ifdef _GENAPI3

///  @{
GC_SYMBOL_LINK(GCGetInfo)
DECLARE((GenTL::TL_INFO_CMD iInfoCmd, GenTL::INFO_DATATYPE* piType, void* pBuffer, size_t* piSize))

///  @}
///  \addtogroup TLError  Error handling
///  @{
GC_SYMBOL_LINK(GCGetLastError)
DECLARE((GenTL::GC_ERROR *piErrorCode, char *sErrText, size_t *piSize))
///  @}

///  @}
///  \addtogroup LibInit  Library Initialization Functions
///  @{
GC_SYMBOL_LINK(GCInitLib)
DECLARE((void))
GC_SYMBOL_LINK(GCCloseLib)
DECLARE((void))
///  @}

///  @}
///  \addtogroup PortInterface   Port Functions
///  @{

GC_SYMBOL_LINK(GCReadPort)
DECLARE((GenTL::PORT_HANDLE hPort, uint64_t iAddress, void *pBuffer, size_t *piSize))
GC_SYMBOL_LINK(GCWritePort)
DECLARE((GenTL::PORT_HANDLE hPort, uint64_t iAddress, const void *pBuffer, size_t *piSize))
GC_SYMBOL_LINK(GCGetPortURL)
DECLARE((GenTL::PORT_HANDLE hPort, char *sURL, size_t *piSize))
GC_SYMBOL_LINK(GCGetPortInfo)
DECLARE((GenTL::PORT_HANDLE hPort, GenTL::PORT_INFO_CMD iInfoCmd, GenTL::INFO_DATATYPE *piType, void *pBuffer, size_t *piSize))


///  @}
///  \addtogroup EventSourceInterface Event Source functions
///  @{
GC_SYMBOL_LINK(GCRegisterEvent)
DECLARE((GenTL::EVENTSRC_HANDLE hEventSrc, GenTL::EVENT_TYPE iEventID, GenTL::EVENT_HANDLE *phEvent))
GC_SYMBOL_LINK(GCUnregisterEvent)
DECLARE((GenTL::EVENTSRC_HANDLE hEventSrc, GenTL::EVENT_TYPE iEventID))

///  @}
///  \addtogroup EventInterface Signaling functions
///  @{
GC_SYMBOL_LINK(EventGetData)
DECLARE((GenTL::EVENT_HANDLE hEvent, void *pBuffer, size_t *piSize, uint64_t iTimeout))
GC_SYMBOL_LINK(EventGetDataInfo)
DECLARE((GenTL::EVENT_HANDLE hEvent, const void *pInBuffer, size_t iInSize, GenTL::EVENT_DATA_INFO_CMD iInfoCmd, GenTL::INFO_DATATYPE *piType, void *pOutBuffer, size_t *piOutSize))
GC_SYMBOL_LINK(EventGetInfo)
DECLARE((GenTL::EVENT_HANDLE hEvent, GenTL::EVENT_INFO_CMD iInfoCmd, GenTL::INFO_DATATYPE *piType, void *pBuffer, size_t *piSize))
GC_SYMBOL_LINK(EventFlush)
DECLARE((GenTL::EVENT_HANDLE hEvent))
GC_SYMBOL_LINK(EventKill)
DECLARE((GenTL::EVENT_HANDLE hEvent))


///  \addtogroup TLAccess  TL access functions
///  @{
GC_SYMBOL_LINK(TLOpen)
DECLARE((GenTL::TL_HANDLE *phTL))
GC_SYMBOL_LINK(TLClose)
DECLARE((GenTL::TL_HANDLE hTL))
GC_SYMBOL_LINK(TLGetInfo)
DECLARE((GenTL::TL_HANDLE hTL, GenTL::TL_INFO_CMD iInfoCmd, GenTL::INFO_DATATYPE *piType, void *pBuffer, size_t *piSize))

GC_SYMBOL_LINK(TLGetNumInterfaces)
DECLARE((GenTL::TL_HANDLE hTL, uint32_t *piNumIfaces))
GC_SYMBOL_LINK(TLGetInterfaceID)
DECLARE((GenTL::TL_HANDLE hTL, uint32_t iIndex,  char * sID, size_t *piSize))
GC_SYMBOL_LINK(TLGetInterfaceInfo)
DECLARE((GenTL::TL_HANDLE hTL, const char *sIfaceID, GenTL::INTERFACE_INFO_CMD iInfoCmd, GenTL::INFO_DATATYPE *piType,  void * pBuffer, size_t *piSize))
GC_SYMBOL_LINK(TLOpenInterface)
DECLARE((GenTL::TL_HANDLE hTL, const char *sIfaceID, GenTL::IF_HANDLE *phIface))
GC_SYMBOL_LINK(TLUpdateInterfaceList)
DECLARE((GenTL::TL_HANDLE hTL, bool8_t *pbChanged, uint64_t iTimeout))

///  @}
///  \addtogroup InterfaceAccess  Interface access functions
///  @{
// obsolete

GC_SYMBOL_LINK(IFClose)
DECLARE((GenTL::IF_HANDLE hIface))
GC_SYMBOL_LINK(IFGetInfo)
DECLARE((GenTL::IF_HANDLE hIface, GenTL::INTERFACE_INFO_CMD iInfoCmd, GenTL::INFO_DATATYPE *piType, void * pBuffer, size_t *piSize))

GC_SYMBOL_LINK(IFGetNumDevices)
DECLARE((GenTL::IF_HANDLE hIface, uint32_t *piNumDevices))
GC_SYMBOL_LINK(IFGetDeviceID)
DECLARE((GenTL::IF_HANDLE hIface, uint32_t iIndex, char * sIDeviceID, size_t *piSize ))
GC_SYMBOL_LINK(IFUpdateDeviceList)
DECLARE((GenTL::IF_HANDLE hIface, bool8_t *pbChanged, uint64_t iTimeout))
GC_SYMBOL_LINK(IFGetDeviceInfo)
DECLARE((GenTL::IF_HANDLE hIface, const char *sDeviceID, GenTL::DEVICE_INFO_CMD iInfoCmd, GenTL::INFO_DATATYPE *piType, void* pBuffer, size_t *piSize))
GC_SYMBOL_LINK(IFOpenDevice)
DECLARE((GenTL::IF_HANDLE hIface, const char *sDeviceID, GenTL::DEVICE_ACCESS_FLAGS iOpenFlags, GenTL::DEV_HANDLE *phDevive))

///  @}
///  \addtogroup DeviceAccess Device access functions
///  @{

GC_SYMBOL_LINK(DevGetPort)
DECLARE((GenTL::DEV_HANDLE hDevice, GenTL::PORT_HANDLE *phRemoteDevice))
GC_SYMBOL_LINK(DevGetNumDataStreams)
DECLARE((GenTL::DEV_HANDLE hDevice, uint32_t *piNumDataStreams))
GC_SYMBOL_LINK(DevGetDataStreamID)
DECLARE((GenTL::DEV_HANDLE hDevice, uint32_t iIndex, char * sDataStreamID, size_t *piSize ))
GC_SYMBOL_LINK(DevOpenDataStream)
DECLARE((GenTL::DEV_HANDLE hDevice, const char *sDataStreamID, GenTL::DS_HANDLE *phDataStream))
GC_SYMBOL_LINK(DevGetInfo)
DECLARE((GenTL::DEV_HANDLE hDevice, GenTL::DEVICE_INFO_CMD iInfoCmd, GenTL::INFO_DATATYPE *piType, void* pBuffer, size_t *piSize))
GC_SYMBOL_LINK(DevClose)
DECLARE((GenTL::DEV_HANDLE hDevice)) 


/// @}
/// \addtogroup DSFunctions DataStream Functions
/// @{

GC_SYMBOL_LINK(DSAnnounceBuffer)
DECLARE((GenTL::DS_HANDLE hDataStream, void *pBuffer, size_t iSize, void *pPrivate, GenTL::BUFFER_HANDLE *phBuffer))
GC_SYMBOL_LINK(DSAllocAndAnnounceBuffer)
DECLARE((GenTL::DS_HANDLE hDataStream, size_t iSize, void *pPrivate, GenTL::BUFFER_HANDLE *phBuffer))
GC_SYMBOL_LINK(DSFlushQueue)
DECLARE((GenTL::DS_HANDLE hDataStream, GenTL::ACQ_QUEUE_TYPE iOperation))
GC_SYMBOL_LINK(DSStartAcquisition)
DECLARE((GenTL::DS_HANDLE hDataStream, GenTL::ACQ_START_FLAGS iStartFlags, uint64_t iNumToAcquire))
GC_SYMBOL_LINK(DSStopAcquisition)
DECLARE((GenTL::DS_HANDLE hDataStream, GenTL::ACQ_STOP_FLAGS iStopFlags))
GC_SYMBOL_LINK(DSGetInfo)
DECLARE((GenTL::DS_HANDLE hDataStream, GenTL::STREAM_INFO_CMD iInfoCmd, GenTL::INFO_DATATYPE *piType, void *pBuffer, size_t *piSize))
GC_SYMBOL_LINK(DSGetBufferID)
DECLARE((GenTL::DS_HANDLE hDataStream, uint32_t iIndex, GenTL::BUFFER_HANDLE *phBuffer))
GC_SYMBOL_LINK(DSClose)
DECLARE((GenTL::DS_HANDLE hDataStream))

GC_SYMBOL_LINK(DSRevokeBuffer)
DECLARE((GenTL::DS_HANDLE hDataStream, GenTL::BUFFER_HANDLE hBuffer, void **pBuffer, void **pPrivate))
GC_SYMBOL_LINK(DSQueueBuffer)
DECLARE((GenTL::DS_HANDLE hDataStream, GenTL::BUFFER_HANDLE hBuffer))
GC_SYMBOL_LINK(DSGetBufferInfo)
DECLARE((GenTL::DS_HANDLE hDataStream, GenTL::BUFFER_HANDLE hBuffer, GenTL::BUFFER_INFO_CMD iInfoCmd, GenTL::INFO_DATATYPE *piType, void *pBuffer, size_t *piSize))

/* GenTL v1.1 */
GC_SYMBOL_LINK(GCGetNumPortURLs)
DECLARE((GenTL::PORT_HANDLE hPort, uint32_t *iNumURLs))
GC_SYMBOL_LINK(GCGetPortURLInfo)
DECLARE((GenTL::PORT_HANDLE hPort, uint32_t iURLIndex, URL_INFO_CMD iInfoCmd, INFO_DATATYPE *piType, void *pBuffer, size_t *piSize))
GC_SYMBOL_LINK(GCReadPortStacked)
DECLARE((GenTL::PORT_HANDLE hPort, GenTL::PORT_REGISTER_STACK_ENTRY *pEntries, size_t *piNumEntries))
GC_SYMBOL_LINK(GCWritePortStacked)
DECLARE((GenTL::PORT_HANDLE hPort, GenTL::PORT_REGISTER_STACK_ENTRY *pEntries, size_t *piNumEntries))

/* GenTL v1.3 */
GC_SYMBOL_LINK(DSGetBufferChunkData)
DECLARE((GenTL::DS_HANDLE hDataStream, GenTL::BUFFER_HANDLE hBuffer, GenTL::SINGLE_CHUNK_DATA *pChunkData, size_t *piNumChunks))


#else
///  @{
GC_SYMBOL_LINK(GCGetInfo)
DECLARE((GenICam::Client::TL_INFO_CMD iInfoCmd, GenICam::Client::INFO_DATATYPE* piType, void* pBuffer, size_t* piSize))

///  @}
///  \addtogroup TLError  Error handling
///  @{
GC_SYMBOL_LINK(GCGetLastError)
DECLARE((GenICam::Client::GC_ERROR *piErrorCode, char *sErrText, size_t *piSize))
///  @}

///  @}
///  \addtogroup LibInit  Library Initialization Functions
///  @{
GC_SYMBOL_LINK(GCInitLib)
DECLARE((void))
GC_SYMBOL_LINK(GCCloseLib)
DECLARE((void))
///  @}

///  @}
///  \addtogroup PortInterface   Port Functions
///  @{

GC_SYMBOL_LINK(GCReadPort)
DECLARE((GenICam::Client::PORT_HANDLE hPort, uint64_t iAddress, void *pBuffer, size_t *piSize))
GC_SYMBOL_LINK(GCWritePort)
DECLARE((GenICam::Client::PORT_HANDLE hPort, uint64_t iAddress, const void *pBuffer, size_t *piSize))
GC_SYMBOL_LINK(GCGetPortURL)
DECLARE((GenICam::Client::PORT_HANDLE hPort, char *sURL, size_t *piSize))
GC_SYMBOL_LINK(GCGetPortInfo)
DECLARE((GenICam::Client::PORT_HANDLE hPort, GenICam::Client::PORT_INFO_CMD iInfoCmd, GenICam::Client::INFO_DATATYPE *piType, void *pBuffer, size_t *piSize))


///  @}
///  \addtogroup EventSourceInterface Event Source functions
///  @{
GC_SYMBOL_LINK(GCRegisterEvent)
DECLARE((GenICam::Client::EVENTSRC_HANDLE hEventSrc, GenICam::Client::EVENT_TYPE iEventID, GenICam::Client::EVENT_HANDLE *phEvent))
GC_SYMBOL_LINK(GCUnregisterEvent)
DECLARE((GenICam::Client::EVENTSRC_HANDLE hEventSrc, GenICam::Client::EVENT_TYPE iEventID))

///  @}
///  \addtogroup EventInterface Signaling functions
///  @{
GC_SYMBOL_LINK(EventGetData)
DECLARE((GenICam::Client::EVENT_HANDLE hEvent, void *pBuffer, size_t *piSize, uint64_t iTimeout))
GC_SYMBOL_LINK(EventGetDataInfo)
DECLARE((GenICam::Client::EVENT_HANDLE hEvent, const void *pInBuffer, size_t iInSize, GenICam::Client::EVENT_DATA_INFO_CMD iInfoCmd, GenICam::Client::INFO_DATATYPE *piType, void *pOutBuffer, size_t *piOutSize))
GC_SYMBOL_LINK(EventGetInfo)
DECLARE((GenICam::Client::EVENT_HANDLE hEvent, GenICam::Client::EVENT_INFO_CMD iInfoCmd, GenICam::Client::INFO_DATATYPE *piType, void *pBuffer, size_t *piSize))
GC_SYMBOL_LINK(EventFlush)
DECLARE((GenICam::Client::EVENT_HANDLE hEvent))
GC_SYMBOL_LINK(EventKill)
DECLARE((GenICam::Client::EVENT_HANDLE hEvent))


///  \addtogroup TLAccess  TL access functions
///  @{
GC_SYMBOL_LINK(TLOpen)
DECLARE((GenICam::Client::TL_HANDLE *phTL))
GC_SYMBOL_LINK(TLClose)
DECLARE((GenICam::Client::TL_HANDLE hTL))
GC_SYMBOL_LINK(TLGetInfo)
DECLARE((GenICam::Client::TL_HANDLE hTL, GenICam::Client::TL_INFO_CMD iInfoCmd, GenICam::Client::INFO_DATATYPE *piType, void *pBuffer, size_t *piSize))

GC_SYMBOL_LINK(TLGetNumInterfaces)
DECLARE((GenICam::Client::TL_HANDLE hTL, uint32_t *piNumIfaces))
GC_SYMBOL_LINK(TLGetInterfaceID)
DECLARE((GenICam::Client::TL_HANDLE hTL, uint32_t iIndex,  char * sID, size_t *piSize))
GC_SYMBOL_LINK(TLGetInterfaceInfo)
DECLARE((GenICam::Client::TL_HANDLE hTL, const char *sIfaceID, GenICam::Client::INTERFACE_INFO_CMD iInfoCmd, GenICam::Client::INFO_DATATYPE *piType,  void * pBuffer, size_t *piSize))
GC_SYMBOL_LINK(TLOpenInterface)
DECLARE((GenICam::Client::TL_HANDLE hTL, const char *sIfaceID, GenICam::Client::IF_HANDLE *phIface))
GC_SYMBOL_LINK(TLUpdateInterfaceList)
DECLARE((GenICam::Client::TL_HANDLE hTL, bool8_t *pbChanged, uint64_t iTimeout))

///  @}
///  \addtogroup InterfaceAccess  Interface access functions
///  @{
// obsolete

GC_SYMBOL_LINK(IFClose)
DECLARE((GenICam::Client::IF_HANDLE hIface))
GC_SYMBOL_LINK(IFGetInfo)
DECLARE((GenICam::Client::IF_HANDLE hIface, GenICam::Client::INTERFACE_INFO_CMD iInfoCmd, GenICam::Client::INFO_DATATYPE *piType, void * pBuffer, size_t *piSize))

GC_SYMBOL_LINK(IFGetNumDevices)
DECLARE((GenICam::Client::IF_HANDLE hIface, uint32_t *piNumDevices))
GC_SYMBOL_LINK(IFGetDeviceID)
DECLARE((GenICam::Client::IF_HANDLE hIface, uint32_t iIndex, char * sIDeviceID, size_t *piSize ))
GC_SYMBOL_LINK(IFUpdateDeviceList)
DECLARE((GenICam::Client::IF_HANDLE hIface, bool8_t *pbChanged, uint64_t iTimeout))
GC_SYMBOL_LINK(IFGetDeviceInfo)
DECLARE((GenICam::Client::IF_HANDLE hIface, const char *sDeviceID, GenICam::Client::DEVICE_INFO_CMD iInfoCmd, GenICam::Client::INFO_DATATYPE *piType, void* pBuffer, size_t *piSize))
GC_SYMBOL_LINK(IFOpenDevice)
DECLARE((GenICam::Client::IF_HANDLE hIface, const char *sDeviceID, GenICam::Client::DEVICE_ACCESS_FLAGS iOpenFlags, GenICam::Client::DEV_HANDLE *phDevive))

///  @}
///  \addtogroup DeviceAccess Device access functions
///  @{

GC_SYMBOL_LINK(DevGetPort)
DECLARE((GenICam::Client::DEV_HANDLE hDevice, GenICam::Client::PORT_HANDLE *phRemoteDevice))
GC_SYMBOL_LINK(DevGetNumDataStreams)
DECLARE((GenICam::Client::DEV_HANDLE hDevice, uint32_t *piNumDataStreams))
GC_SYMBOL_LINK(DevGetDataStreamID)
DECLARE((GenICam::Client::DEV_HANDLE hDevice, uint32_t iIndex, char * sDataStreamID, size_t *piSize ))
GC_SYMBOL_LINK(DevOpenDataStream)
DECLARE((GenICam::Client::DEV_HANDLE hDevice, const char *sDataStreamID, GenICam::Client::DS_HANDLE *phDataStream))
GC_SYMBOL_LINK(DevGetInfo)
DECLARE((GenICam::Client::DEV_HANDLE hDevice, GenICam::Client::DEVICE_INFO_CMD iInfoCmd, GenICam::Client::INFO_DATATYPE *piType, void* pBuffer, size_t *piSize))
GC_SYMBOL_LINK(DevClose)
DECLARE((GenICam::Client::DEV_HANDLE hDevice)) 


/// @}
/// \addtogroup DSFunctions DataStream Functions
/// @{

GC_SYMBOL_LINK(DSAnnounceBuffer)
DECLARE((GenICam::Client::DS_HANDLE hDataStream, void *pBuffer, size_t iSize, void *pPrivate, GenICam::Client::BUFFER_HANDLE *phBuffer))
GC_SYMBOL_LINK(DSAllocAndAnnounceBuffer)
DECLARE((GenICam::Client::DS_HANDLE hDataStream, size_t iSize, void *pPrivate, GenICam::Client::BUFFER_HANDLE *phBuffer))
GC_SYMBOL_LINK(DSFlushQueue)
DECLARE((GenICam::Client::DS_HANDLE hDataStream, GenICam::Client::ACQ_QUEUE_TYPE iOperation))
GC_SYMBOL_LINK(DSStartAcquisition)
DECLARE((GenICam::Client::DS_HANDLE hDataStream, GenICam::Client::ACQ_START_FLAGS iStartFlags, uint64_t iNumToAcquire))
GC_SYMBOL_LINK(DSStopAcquisition)
DECLARE((GenICam::Client::DS_HANDLE hDataStream, GenICam::Client::ACQ_STOP_FLAGS iStopFlags))
GC_SYMBOL_LINK(DSGetInfo)
DECLARE((GenICam::Client::DS_HANDLE hDataStream, GenICam::Client::STREAM_INFO_CMD iInfoCmd, GenICam::Client::INFO_DATATYPE *piType, void *pBuffer, size_t *piSize))
GC_SYMBOL_LINK(DSGetBufferID)
DECLARE((GenICam::Client::DS_HANDLE hDataStream, uint32_t iIndex, GenICam::Client::BUFFER_HANDLE *phBuffer))
GC_SYMBOL_LINK(DSClose)
DECLARE((GenICam::Client::DS_HANDLE hDataStream))

GC_SYMBOL_LINK(DSRevokeBuffer)
DECLARE((GenICam::Client::DS_HANDLE hDataStream, GenICam::Client::BUFFER_HANDLE hBuffer, void **pBuffer, void **pPrivate))
GC_SYMBOL_LINK(DSQueueBuffer)
DECLARE((GenICam::Client::DS_HANDLE hDataStream, GenICam::Client::BUFFER_HANDLE hBuffer))
GC_SYMBOL_LINK(DSGetBufferInfo)
DECLARE((GenICam::Client::DS_HANDLE hDataStream, GenICam::Client::BUFFER_HANDLE hBuffer, GenICam::Client::BUFFER_INFO_CMD iInfoCmd, GenICam::Client::INFO_DATATYPE *piType, void *pBuffer, size_t *piSize))

/* GenTL v1.1 */
GC_SYMBOL_LINK(GCGetNumPortURLs)
DECLARE((GenICam::Client::PORT_HANDLE hPort, uint32_t *iNumURLs))
GC_SYMBOL_LINK(GCGetPortURLInfo)
DECLARE((GenICam::Client::PORT_HANDLE hPort, uint32_t iURLIndex, URL_INFO_CMD iInfoCmd, INFO_DATATYPE *piType, void *pBuffer, size_t *piSize))
GC_SYMBOL_LINK(GCReadPortStacked)
DECLARE((GenICam::Client::PORT_HANDLE hPort, GenICam::Client::PORT_REGISTER_STACK_ENTRY *pEntries, size_t *piNumEntries))
GC_SYMBOL_LINK(GCWritePortStacked)
DECLARE((GenICam::Client::PORT_HANDLE hPort, GenICam::Client::PORT_REGISTER_STACK_ENTRY *pEntries, size_t *piNumEntries))

/* GenTL v1.3 */
GC_SYMBOL_LINK(DSGetBufferChunkData)
DECLARE((GenICam::Client::DS_HANDLE hDataStream, GenICam::Client::BUFFER_HANDLE hBuffer, GenICam::Client::SINGLE_CHUNK_DATA *pChunkData, size_t *piNumChunks))

#endif

/* GenTL v1.4 */
//AvalのGenTLが対応していないため、削除（TLOpenできなくなる）
//GC_SYMBOL_LINK(IFGetParentTL)
//DECLARE((GenICam::Client::IF_HANDLE hIface, GenICam::Client::TL_HANDLE *phSystem))
//GC_SYMBOL_LINK(DevGetParentIF)
//DECLARE((GenICam::Client::DEV_HANDLE hDevice, GenICam::Client::IF_HANDLE *phIface))
//GC_SYMBOL_LINK(DSGetParentDev)
//DECLARE((GenICam::Client::DS_HANDLE hDataStream, GenICam::Client::DEV_HANDLE *phDevice))



/// @}