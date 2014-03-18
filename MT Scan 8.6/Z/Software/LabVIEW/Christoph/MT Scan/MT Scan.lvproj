<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="9008000">
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
		<Item Name="SubVIs" Type="Folder" URL="../SubVIs">
			<Property Name="NI.DISK" Type="Bool">true</Property>
		</Item>
		<Item Name="MTS_Main.vi" Type="VI" URL="../MTS_Main.vi"/>
		<Item Name="MT Scan.lvproj" Type="Document" URL="/Z/Software/LabVIEW/Christoph/MT Scan/MT Scan.lvproj"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="user.lib" Type="Folder">
				<Item Name="Global1.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/Global1.vi"/>
				<Item Name="GCSTranslator DLL Functions.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/GCSTranslator DLL Functions.vi"/>
				<Item Name="Close connection if open.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/Close connection if open.vi"/>
				<Item Name="Controller names.ctl" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/Controller names.ctl"/>
				<Item Name="Global2.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/Global2.vi"/>
				<Item Name="PI ReceiveString GPIB.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/PI ReceiveString GPIB.vi"/>
				<Item Name="PI ReceiveNCharacters RS232.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/PI ReceiveNCharacters RS232.vi"/>
				<Item Name="PI Receive String.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/PI Receive String.vi"/>
				<Item Name="Build command substring.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Build command substring.vi"/>
				<Item Name="Get arrays without blanks.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Get arrays without blanks.vi"/>
				<Item Name="Commanded axes connected?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Commanded axes connected?.vi"/>
				<Item Name="MOV.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/MOV.vi"/>
				<Item Name="Assign values from string to axes.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Assign values from string to axes.vi"/>
				<Item Name="Build query command substring.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Build query command substring.vi"/>
				<Item Name="POS?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/POS?.vi"/>
				<Item Name="SVO.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/SVO.vi"/>
				<Item Name="Assign booleans from string to axes.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Assign booleans from string to axes.vi"/>
				<Item Name="SVO?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/SVO?.vi"/>
				<Item Name="Select with boolean array input.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Select with boolean array input.vi"/>
				<Item Name="ERR?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/ERR?.vi"/>
				<Item Name="Get lines from string.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Get lines from string.vi"/>
				<Item Name="VST?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/VST?.vi"/>
				<Item Name="Commanded stage name available?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Commanded stage name available?.vi"/>
				<Item Name="CST.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/CST.vi"/>
				<Item Name="GCSTranslateError.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/GCSTranslateError.vi"/>
				<Item Name="SAI?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/SAI?.vi"/>
				<Item Name="TMN?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Limits.llb/TMN?.vi"/>
				<Item Name="TMX?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Limits.llb/TMX?.vi"/>
				<Item Name="ONT?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/ONT?.vi"/>
				<Item Name="#5.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/#5.vi"/>
				<Item Name="#5_old.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Old commands.llb/#5_old.vi"/>
				<Item Name="Wait for hexapod system axes to stop.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Old commands.llb/Wait for hexapod system axes to stop.vi"/>
				<Item Name="STA?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/STA?.vi"/>
				<Item Name="Wait for axes to stop.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Wait for axes to stop.vi"/>
				<Item Name="General wait for movement to stop.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/General wait for movement to stop.vi"/>
				<Item Name="Select values for chosen axes.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Select values for chosen axes.vi"/>
				<Item Name="INI.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/INI.vi"/>
				<Item Name="#24.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/#24.vi"/>
				<Item Name="#7.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Special command.llb/#7.vi"/>
				<Item Name="Wait for answer of longlasting command.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Wait for answer of longlasting command.vi"/>
				<Item Name="Longlasting one-axis command.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Longlasting one-axis command.vi"/>
				<Item Name="ATZ.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Limits.llb/ATZ.vi"/>
				<Item Name="Substract axes array subset from axes array.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Substract axes array subset from axes array.vi"/>
				<Item Name="Define axes to command from boolean array.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Define axes to command from boolean array.vi"/>
				<Item Name="Define connected systems.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/Define connected systems.vi"/>
				<Item Name="PI Open Interface of one system.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/PI Open Interface of one system.vi"/>
				<Item Name="Define connected axes.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/Define connected axes.vi"/>
				<Item Name="*IDN?.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/General command.llb/*IDN?.vi"/>
				<Item Name="E710_Configuration_Setup.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/E710_Configuration_Setup.vi"/>
				<Item Name="GCSTranslator.dll" Type="Document" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/GCSTranslator.dll"/>
				<Item Name="PI Send String.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Communication.llb/PI Send String.vi"/>
				<Item Name="Build num command substring.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/Support.llb/Build num command substring.vi"/>
				<Item Name="#9.vi" Type="VI" URL="/&lt;userlib&gt;/E710_GCS_LabVIEW/Low Level/WaveGenerator.llb/#9.vi"/>
			</Item>
			<Item Name="vi.lib" Type="Folder">
				<Item Name="serpConfig.vi" Type="VI" URL="/&lt;vilib&gt;/Instr/serial.llb/serpConfig.vi"/>
				<Item Name="Open Serial Driver.vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_sersup.llb/Open Serial Driver.vi"/>
				<Item Name="Serial Port Read.vi" Type="VI" URL="/&lt;vilib&gt;/Instr/serial.llb/Serial Port Read.vi"/>
				<Item Name="Bytes At Serial Port.vi" Type="VI" URL="/&lt;vilib&gt;/Instr/serial.llb/Bytes At Serial Port.vi"/>
				<Item Name="Serial Port Write.vi" Type="VI" URL="/&lt;vilib&gt;/Instr/serial.llb/Serial Port Write.vi"/>
				<Item Name="Space Constant.vi" Type="VI" URL="/&lt;vilib&gt;/dlg_ctls.llb/Space Constant.vi"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="VISA Configure Serial Port (Instr).vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port (Instr).vi"/>
				<Item Name="VISA Configure Serial Port (Serial Instr).vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port (Serial Instr).vi"/>
				<Item Name="VISA Configure Serial Port" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port"/>
				<Item Name="Serial Port Init.vi" Type="VI" URL="/&lt;vilib&gt;/Instr/serial.llb/Serial Port Init.vi"/>
				<Item Name="Merge Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Merge Errors.vi"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="Simple Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Simple Error Handler.vi"/>
				<Item Name="DialogType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogType.ctl"/>
				<Item Name="General Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler.vi"/>
				<Item Name="DialogTypeEnum.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogTypeEnum.ctl"/>
				<Item Name="General Error Handler CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler CORE.vi"/>
				<Item Name="Check Special Tags.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Check Special Tags.vi"/>
				<Item Name="TagReturnType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/TagReturnType.ctl"/>
				<Item Name="Set String Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set String Value.vi"/>
				<Item Name="GetRTHostConnectedProp.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetRTHostConnectedProp.vi"/>
				<Item Name="Error Code Database.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Code Database.vi"/>
				<Item Name="Format Message String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Format Message String.vi"/>
				<Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
				<Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
				<Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
				<Item Name="Details Display Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Details Display Dialog.vi"/>
				<Item Name="ErrWarn.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/ErrWarn.ctl"/>
				<Item Name="eventvkey.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/eventvkey.ctl"/>
				<Item Name="Not Found Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Not Found Dialog.vi"/>
				<Item Name="Three Button Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog.vi"/>
				<Item Name="Three Button Dialog CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog CORE.vi"/>
				<Item Name="Longest Line Length in Pixels.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Longest Line Length in Pixels.vi"/>
				<Item Name="Convert property node font to graphics font.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Convert property node font to graphics font.vi"/>
				<Item Name="Get Text Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Get Text Rect.vi"/>
				<Item Name="Get String Text Bounds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Get String Text Bounds.vi"/>
				<Item Name="LVBoundsTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVBoundsTypeDef.ctl"/>
				<Item Name="BuildHelpPath.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/BuildHelpPath.vi"/>
				<Item Name="GetHelpDir.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetHelpDir.vi"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="subFile Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/express/express input/FileDialogBlock.llb/subFile Dialog.vi"/>
				<Item Name="ex_CorrectErrorChain.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/ex_CorrectErrorChain.vi"/>
				<Item Name="NI_LVConfig.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/config.llb/NI_LVConfig.lvlib"/>
				<Item Name="NI_FileType.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/lvfile.llb/NI_FileType.lvlib"/>
				<Item Name="Check if File or Folder Exists.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Check if File or Folder Exists.vi"/>
				<Item Name="IMAQ Image.ctl" Type="VI" URL="/&lt;vilib&gt;/vision/Image Controls.llb/IMAQ Image.ctl"/>
				<Item Name="IMAQ ArrayToImage" Type="VI" URL="/&lt;vilib&gt;/vision/Pixel Manipulation.llb/IMAQ ArrayToImage"/>
				<Item Name="IMAQ Create" Type="VI" URL="/&lt;vilib&gt;/vision/Basics.llb/IMAQ Create"/>
				<Item Name="Image Type" Type="VI" URL="/&lt;vilib&gt;/vision/Image Controls.llb/Image Type"/>
				<Item Name="IMAQ Dispose" Type="VI" URL="/&lt;vilib&gt;/vision/Basics.llb/IMAQ Dispose"/>
				<Item Name="IMAQ ArrayToColorImage" Type="VI" URL="/&lt;vilib&gt;/vision/Color.llb/IMAQ ArrayToColorImage"/>
				<Item Name="Color (U64)" Type="VI" URL="/&lt;vilib&gt;/vision/Image Controls.llb/Color (U64)"/>
				<Item Name="IMAQ Write File 2" Type="VI" URL="/&lt;vilib&gt;/vision/Files.llb/IMAQ Write File 2"/>
				<Item Name="IMAQ Write BMP File 2" Type="VI" URL="/&lt;vilib&gt;/vision/Files.llb/IMAQ Write BMP File 2"/>
				<Item Name="IMAQ Write Image And Vision Info File 2" Type="VI" URL="/&lt;vilib&gt;/vision/Files.llb/IMAQ Write Image And Vision Info File 2"/>
				<Item Name="IMAQ Write JPEG File 2" Type="VI" URL="/&lt;vilib&gt;/vision/Files.llb/IMAQ Write JPEG File 2"/>
				<Item Name="IMAQ Write JPEG2000 File 2" Type="VI" URL="/&lt;vilib&gt;/vision/Files.llb/IMAQ Write JPEG2000 File 2"/>
				<Item Name="IMAQ Write PNG File 2" Type="VI" URL="/&lt;vilib&gt;/vision/Files.llb/IMAQ Write PNG File 2"/>
				<Item Name="IMAQ Write TIFF File 2" Type="VI" URL="/&lt;vilib&gt;/vision/Files.llb/IMAQ Write TIFF File 2"/>
			</Item>
			<Item Name="user32" Type="VI" URL="user32">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="kernel32" Type="VI" URL="kernel32">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="nivision.dll" Type="Document" URL="nivision.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
			<Item Name="nivissvc.dll" Type="Document" URL="nivissvc.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
