<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="10008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="newMicroTime.vi" Type="VI" URL="../newMicroTime.vi"/>
		<Item Name="PI_MoveTo.vi" Type="VI" URL="../../MT Scan/SubVIs/PI_MoveTo.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="user.lib" Type="Folder">
				<Item Name="#5.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/#5.vi"/>
				<Item Name="#5_old.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Old commands.llb/#5_old.vi"/>
				<Item Name="#7.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/#7.vi"/>
				<Item Name="#24.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/#24.vi"/>
				<Item Name="*IDN?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/*IDN?.vi"/>
				<Item Name="Assign booleans from string to axes.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Assign booleans from string to axes.vi"/>
				<Item Name="Assign values from string to axes.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Assign values from string to axes.vi"/>
				<Item Name="ATZ.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Limits.llb/ATZ.vi"/>
				<Item Name="Build command substring.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Build command substring.vi"/>
				<Item Name="Build query command substring.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Build query command substring.vi"/>
				<Item Name="Close connection if open.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/Close connection if open.vi"/>
				<Item Name="Commanded axes connected?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Commanded axes connected?.vi"/>
				<Item Name="Commanded stage name available?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Commanded stage name available?.vi"/>
				<Item Name="Controller names.ctl" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/Controller names.ctl"/>
				<Item Name="CST.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/CST.vi"/>
				<Item Name="Define axes to command from boolean array.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Define axes to command from boolean array.vi"/>
				<Item Name="Define connected axes.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/Define connected axes.vi"/>
				<Item Name="Define connected systems.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/Define connected systems.vi"/>
				<Item Name="E710_Configuration_Setup.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/E710_Configuration_Setup.vi"/>
				<Item Name="ERR?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/ERR?.vi"/>
				<Item Name="GCSTranslateError.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/GCSTranslateError.vi"/>
				<Item Name="GCSTranslator DLL Functions.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/GCSTranslator DLL Functions.vi"/>
				<Item Name="GCSTranslator.dll" Type="Document" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/GCSTranslator.dll"/>
				<Item Name="General wait for movement to stop.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/General wait for movement to stop.vi"/>
				<Item Name="Get arrays without blanks.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Get arrays without blanks.vi"/>
				<Item Name="Get lines from string.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Get lines from string.vi"/>
				<Item Name="Global1.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/Global1.vi"/>
				<Item Name="Global2.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/Global2.vi"/>
				<Item Name="INI.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/INI.vi"/>
				<Item Name="Longlasting one-axis command.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Longlasting one-axis command.vi"/>
				<Item Name="MOV.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/MOV.vi"/>
				<Item Name="ONT?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/ONT?.vi"/>
				<Item Name="PI Open Interface of one system.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/PI Open Interface of one system.vi"/>
				<Item Name="PI Receive String.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/PI Receive String.vi"/>
				<Item Name="PI ReceiveNCharacters RS232.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/PI ReceiveNCharacters RS232.vi"/>
				<Item Name="PI ReceiveString GPIB.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/PI ReceiveString GPIB.vi"/>
				<Item Name="PI Send String.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/PI Send String.vi"/>
				<Item Name="POS?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/POS?.vi"/>
				<Item Name="SAI?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/SAI?.vi"/>
				<Item Name="Select values for chosen axes.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Select values for chosen axes.vi"/>
				<Item Name="Select with boolean array input.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Select with boolean array input.vi"/>
				<Item Name="STA?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/STA?.vi"/>
				<Item Name="Substract axes array subset from axes array.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Substract axes array subset from axes array.vi"/>
				<Item Name="SVO.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/SVO.vi"/>
				<Item Name="SVO?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/SVO?.vi"/>
				<Item Name="TMN?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Limits.llb/TMN?.vi"/>
				<Item Name="TMX?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Limits.llb/TMX?.vi"/>
				<Item Name="VST?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/VST?.vi"/>
				<Item Name="Wait for answer of longlasting command.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Wait for answer of longlasting command.vi"/>
				<Item Name="Wait for axes to stop.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Wait for axes to stop.vi"/>
				<Item Name="Wait for hexapod system axes to stop.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Old commands.llb/Wait for hexapod system axes to stop.vi"/>
				<Item Name="WGO.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/WaveGenerator.llb/WGO.vi"/>
			</Item>
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Bytes At Serial Port.vi" Type="VI" URL="/&lt;vilib&gt;/INSTR/SERIAL.LLB/Bytes At Serial Port.vi"/>
				<Item Name="Check if File or Folder Exists.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Check if File or Folder Exists.vi"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="Color (U64)" Type="VI" URL="/&lt;vilib&gt;/vision/Image Controls.llb/Color (U64)"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="ex_CorrectErrorChain.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/ex_CorrectErrorChain.vi"/>
				<Item Name="Image Type" Type="VI" URL="/&lt;vilib&gt;/vision/Image Controls.llb/Image Type"/>
				<Item Name="IMAQ ArrayToColorImage" Type="VI" URL="/&lt;vilib&gt;/vision/Basics.llb/IMAQ ArrayToColorImage"/>
				<Item Name="IMAQ ArrayToImage" Type="VI" URL="/&lt;vilib&gt;/vision/Basics.llb/IMAQ ArrayToImage"/>
				<Item Name="IMAQ Create" Type="VI" URL="/&lt;vilib&gt;/vision/Basics.llb/IMAQ Create"/>
				<Item Name="IMAQ Dispose" Type="VI" URL="/&lt;vilib&gt;/vision/Basics.llb/IMAQ Dispose"/>
				<Item Name="IMAQ Image.ctl" Type="VI" URL="/&lt;vilib&gt;/vision/Image Controls.llb/IMAQ Image.ctl"/>
				<Item Name="NI_FileType.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/lvfile.llb/NI_FileType.lvlib"/>
				<Item Name="NI_LVConfig.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/config.llb/NI_LVConfig.lvlib"/>
				<Item Name="NI_Vision_Development_Module.lvlib" Type="Library" URL="/&lt;vilib&gt;/vision/NI_Vision_Development_Module.lvlib"/>
				<Item Name="Open Serial Driver.vi" Type="VI" URL="/&lt;vilib&gt;/instr/_sersup.llb/Open Serial Driver.vi"/>
				<Item Name="Serial Port Init.vi" Type="VI" URL="/&lt;vilib&gt;/INSTR/SERIAL.LLB/Serial Port Init.vi"/>
				<Item Name="Serial Port Read.vi" Type="VI" URL="/&lt;vilib&gt;/Instr/Serial.llb/Serial Port Read.vi"/>
				<Item Name="Serial Port Write.vi" Type="VI" URL="/&lt;vilib&gt;/INSTR/SERIAL.LLB/Serial Port Write.vi"/>
				<Item Name="serpConfig.vi" Type="VI" URL="/&lt;vilib&gt;/Instr/serial.llb/serpConfig.vi"/>
				<Item Name="Space Constant.vi" Type="VI" URL="/&lt;vilib&gt;/dlg_ctls.llb/Space Constant.vi"/>
				<Item Name="subFile Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/express/express input/FileDialogBlock.llb/subFile Dialog.vi"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="VISA Configure Serial Port" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port"/>
				<Item Name="VISA Configure Serial Port (Instr).vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port (Instr).vi"/>
				<Item Name="VISA Configure Serial Port (Serial Instr).vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port (Serial Instr).vi"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
			</Item>
			<Item Name="CAN PCI Close.vi" Type="VI" URL="../../MT Scan/SubVIs/PEAK CAN PCI.llb/CAN PCI Close.vi"/>
			<Item Name="CAN PCI Init.vi" Type="VI" URL="../../MT Scan/SubVIs/PEAK CAN PCI.llb/CAN PCI Init.vi"/>
			<Item Name="CAN PCI Read.vi" Type="VI" URL="../../MT Scan/SubVIs/PEAK CAN PCI.llb/CAN PCI Read.vi"/>
			<Item Name="CAN PCI Write.vi" Type="VI" URL="../../MT Scan/SubVIs/PEAK CAN PCI.llb/CAN PCI Write.vi"/>
			<Item Name="CAN USB Write.vi" Type="VI" URL="../SubVIs/PEAK CAN USB.llb/CAN USB Write.vi"/>
			<Item Name="ConnectedAxes.vi" Type="VI" URL="../Variables/ConnectedAxes.vi"/>
			<Item Name="Control.ctl" Type="VI" URL="../Controls/Control.ctl"/>
			<Item Name="CreateNewWSfile.vi" Type="VI" URL="../GUI/CreateNewWSfile.vi"/>
			<Item Name="DT3120_AquireFrame.vi" Type="VI" URL="../SubVIs/DT3120_AquireFrame.vi"/>
			<Item Name="DT3120_BArrayToFrameInfoStruct.vi" Type="VI" URL="../SubVIs/DT3120_BArrayToFrameInfoStruct.vi"/>
			<Item Name="DT3120_BArrayToInfoStruct.vi" Type="VI" URL="../SubVIs/DT3120_BArrayToInfoStruct.vi"/>
			<Item Name="DT3120_CheckStatus.vi" Type="VI" URL="../SubVIs/DT3120_CheckStatus.vi"/>
			<Item Name="DT3120_ChopU16FromBArray.vi" Type="VI" URL="../SubVIs/DT3120_ChopU16FromBArray.vi"/>
			<Item Name="DT3120_ChopU32FromBArray.vi" Type="VI" URL="../SubVIs/DT3120_ChopU32FromBArray.vi"/>
			<Item Name="DT3120_CloseDevice.vi" Type="VI" URL="../SubVIs/DT3120_CloseDevice.vi"/>
			<Item Name="DT3120_FG_SINGLE_FRAME_OPS.vi" Type="VI" URL="../SubVIs/DT3120_FG_SINGLE_FRAME_OPS.vi"/>
			<Item Name="DT3120_GetFrameSize.vi" Type="VI" URL="../SubVIs/DT3120_GetFrameSize.vi"/>
			<Item Name="DT3120_GetInputSourceList.vi" Type="VI" URL="../SubVIs/DT3120_GetInputSourceList.vi"/>
			<Item Name="DT3120_InitDeviceList.vi" Type="VI" URL="../SubVIs/DT3120_InitDeviceList.vi"/>
			<Item Name="DT3120_NewDevice.vi" Type="VI" URL="../SubVIs/DT3120_NewDevice.vi"/>
			<Item Name="DT3120_PassthruModes.vi" Type="VI" URL="../SubVIs/DT3120_PassthruModes.vi"/>
			<Item Name="GetCountRates.vi" Type="VI" URL="../HydraHarp/GetCountRates.vi"/>
			<Item Name="GetPicture.vi" Type="VI" URL="../Variables/GetPicture.vi"/>
			<Item Name="GlobalLaserInterface.vi" Type="VI" URL="../Variables/GlobalLaserInterface.vi"/>
			<Item Name="HH_AccumCounts.vi" Type="VI" URL="../HydraHarp/HH_AccumCounts.vi"/>
			<Item Name="HH_CheckErrorCode.vi" Type="VI" URL="../HydraHarp/HH_CheckErrorCode.vi"/>
			<Item Name="HH_FIFORead.vi" Type="VI" URL="../HydraHarp/HH_FIFORead.vi"/>
			<Item Name="HH_FileToImage_1.vi" Type="VI" URL="../HydraHarp/HH_FileToImage_1.vi"/>
			<Item Name="HH_FileToImage_2.vi" Type="VI" URL="../HydraHarp/HH_FileToImage_2.vi"/>
			<Item Name="HH_Init.vi" Type="VI" URL="../HydraHarp/HH_Init.vi"/>
			<Item Name="HH_PI_PhysikInstrumenteInitialisiation.vi" Type="VI" URL="../GUI/HH_PI_PhysikInstrumenteInitialisiation.vi"/>
			<Item Name="HH_Set.vi" Type="VI" URL="../HydraHarp/HH_Set.vi"/>
			<Item Name="HH_SetChan.vi" Type="VI" URL="../HydraHarp/HH_SetChan.vi"/>
			<Item Name="HH_SplitRecord.vi" Type="VI" URL="../HydraHarp/HH_SplitRecord.vi"/>
			<Item Name="HH_Start.vi" Type="VI" URL="../HydraHarp/HH_Start.vi"/>
			<Item Name="HH_Stop.vi" Type="VI" URL="../HydraHarp/HH_Stop.vi"/>
			<Item Name="HH_ToColorImage.vi" Type="VI" URL="../HydraHarp/HH_ToColorImage.vi"/>
			<Item Name="HH_ToImage.vi" Type="VI" URL="../HydraHarp/HH_ToImage.vi"/>
			<Item Name="HH_TraceAndHisto.vi" Type="VI" URL="../HydraHarp/HH_TraceAndHisto.vi"/>
			<Item Name="hhlib.dll" Type="Document" URL="hhlib.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="HT3_ByteArrayToStrs.vi" Type="VI" URL="../HydraHarp/HT3_ByteArrayToStrs.vi"/>
			<Item Name="HT3_Create.vi" Type="VI" URL="../HydraHarp/HT3_Create.vi"/>
			<Item Name="HT3_Finish.vi" Type="VI" URL="../HydraHarp/HT3_Finish.vi"/>
			<Item Name="HT3_ImgHeaderToIntArray.vi" Type="VI" URL="../HydraHarp/HT3_ImgHeaderToIntArray.vi"/>
			<Item Name="HT3_IntArrayToImgHeader.vi" Type="VI" URL="../HydraHarp/HT3_IntArrayToImgHeader.vi"/>
			<Item Name="HT3_Open.vi" Type="VI" URL="../HydraHarp/HT3_Open.vi"/>
			<Item Name="HT3_Read.vi" Type="VI" URL="../HydraHarp/HT3_Read.vi"/>
			<Item Name="HT3_StrsToByteArray.vi" Type="VI" URL="../HydraHarp/HT3_StrsToByteArray.vi"/>
			<Item Name="HT3_Write.vi" Type="VI" URL="../HydraHarp/HT3_Write.vi"/>
			<Item Name="InitScanner.vi" Type="VI" URL="../GUI/InitScanner.vi"/>
			<Item Name="MTS_AddFileToWorkspace.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_AddFileToWorkspace.vi"/>
			<Item Name="MTS_AutoPointCords.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_AutoPointCords.vi"/>
			<Item Name="MTS_AutoZPos.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_AutoZPos.vi"/>
			<Item Name="MTS_BuildWSTree.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_BuildWSTree.vi"/>
			<Item Name="MTS_ClearWSTree2.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_ClearWSTree2.vi"/>
			<Item Name="MTS_CreateWriteMSW.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_CreateWriteMSW.vi"/>
			<Item Name="MTS_DrawCrossIMAQIamge.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_DrawCrossIMAQIamge.vi"/>
			<Item Name="MTS_GenFileComment.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_GenFileComment.vi"/>
			<Item Name="MTS_GenGridScanList.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_GenGridScanList.vi"/>
			<Item Name="MTS_GetFileFromWS.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_GetFileFromWS.vi"/>
			<Item Name="MTS_GetNextFileName.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_GetNextFileName.vi"/>
			<Item Name="MTS_IntensityArrayToImage.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_IntensityArrayToImage.vi"/>
			<Item Name="MTS_PreapareImage.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_PreapareImage.vi"/>
			<Item Name="MTS_ReadMSW.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_ReadMSW.vi"/>
			<Item Name="MTS_ScannerXYZToImageXY.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_ScannerXYZToImageXY.vi"/>
			<Item Name="MTS_SelectTCSPCBinning.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_SelectTCSPCBinning.vi"/>
			<Item Name="MTS_TCSPCBinnAndSum.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_TCSPCBinnAndSum.vi"/>
			<Item Name="MTS_Workspace.vi" Type="VI" URL="../MT Scan/SubVIs/MTS_Workspace.vi"/>
			<Item Name="nivision.dll" Type="Document" URL="nivision.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="nivissvc.dll" Type="Document" URL="nivissvc.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="olfg32.dll" Type="Document" URL="../Libs/olfg32.dll"/>
			<Item Name="olimg32.dll" Type="Document" URL="../Libs/olimg32.dll"/>
			<Item Name="patternImg.vi" Type="VI" URL="../Variables/patternImg.vi"/>
			<Item Name="PCAN_CompMessage.vi" Type="VI" URL="../SubVIs/PCAN_CompMessage.vi"/>
			<Item Name="PCAN_DecompMessage.vi" Type="VI" URL="../SubVIs/PCAN_DecompMessage.vi"/>
			<Item Name="Pcan_pci_orig.dll" Type="Document" URL="../../../Program Files/PicoQuant/SymPhoTime/Pcan_pci_orig.dll"/>
			<Item Name="PI Goto GPIB.vi" Type="VI" URL="../Scanner/PI Goto GPIB.vi"/>
			<Item Name="PI Query GPIB.vi" Type="VI" URL="../Scanner/PI Query GPIB.vi"/>
			<Item Name="PI Start WG.vi" Type="VI" URL="../Scanner/PI Start WG.vi"/>
			<Item Name="PI Write GPIB.vi" Type="VI" URL="../Scanner/PI Write GPIB.vi"/>
			<Item Name="PI_AreaScan_Prepare_1.vi" Type="VI" URL="../Scanner/PI_AreaScan_Prepare_1.vi"/>
			<Item Name="PI_CheckScan.vi" Type="VI" URL="../Scanner/PI_CheckScan.vi"/>
			<Item Name="PI_MoveTo.vi" Type="VI" URL="../MT Scan/SubVIs/PI_MoveTo.vi"/>
			<Item Name="Scan_AlignImage.vi" Type="VI" URL="../Scanner/Scan_AlignImage.vi"/>
			<Item Name="Scan_SetGraphScales.vi" Type="VI" URL="../Scanner/Scan_SetGraphScales.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="newMicroTime" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{54FC51A4-3303-47CB-96C7-4E744798A06C}</Property>
				<Property Name="App_INI_GUID" Type="Str">{DA3F1D1A-90B4-4061-88B5-A930FC5B0AFA}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">newMicroTime</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../newMicroTime/builds/MicroMain</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Destination[0].destName" Type="Str">MicroTimeMain.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../NI_AB_PROJECTNAME/builds/MicroMain/MicroTimeMain.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../NI_AB_PROJECTNAME/builds/MicroMain/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{4693BEB6-288B-4731-A51C-C2321BBDF6D9}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/newMicroTime.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">Uni Goettingen</Property>
				<Property Name="TgtF_fileDescription" Type="Str">newMicroTime</Property>
				<Property Name="TgtF_fileVersion.major" Type="Int">1</Property>
				<Property Name="TgtF_internalName" Type="Str">newMicroTime</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2014 Uni Goettingen</Property>
				<Property Name="TgtF_productName" Type="Str">newMicroTime</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{42E4B722-B2E6-4782-A866-C61B5A45E3D0}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">MicroTimeMain.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
