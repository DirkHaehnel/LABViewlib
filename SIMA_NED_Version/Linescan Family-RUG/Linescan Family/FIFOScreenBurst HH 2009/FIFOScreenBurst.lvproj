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
		<Item Name="FIFOScreenBurst.vi" Type="VI" URL="../FIFOScreenBurst.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Write To Spreadsheet File.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write To Spreadsheet File.vi"/>
				<Item Name="Write To Spreadsheet File (DBL).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write To Spreadsheet File (DBL).vi"/>
				<Item Name="DialogType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogType.ctl"/>
				<Item Name="General Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler.vi"/>
				<Item Name="DialogTypeEnum.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogTypeEnum.ctl"/>
				<Item Name="General Error Handler CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler CORE.vi"/>
				<Item Name="Check Special Tags.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Check Special Tags.vi"/>
				<Item Name="TagReturnType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/TagReturnType.ctl"/>
				<Item Name="Set String Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set String Value.vi"/>
				<Item Name="GetRTHostConnectedProp.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetRTHostConnectedProp.vi"/>
				<Item Name="Error Code Database.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Code Database.vi"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="Format Message String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Format Message String.vi"/>
				<Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
				<Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
				<Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
				<Item Name="Details Display Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Details Display Dialog.vi"/>
				<Item Name="ErrWarn.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/ErrWarn.ctl"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
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
				<Item Name="Write Spreadsheet String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write Spreadsheet String.vi"/>
				<Item Name="Write To Spreadsheet File (I64).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write To Spreadsheet File (I64).vi"/>
				<Item Name="Write To Spreadsheet File (string).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Write To Spreadsheet File (string).vi"/>
				<Item Name="LVCursorListTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVCursorListTypeDef.ctl"/>
				<Item Name="NI_AALBase.lvlib" Type="Library" URL="/&lt;vilib&gt;/Analysis/NI_AALBase.lvlib"/>
				<Item Name="Open_Create_Replace File.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/Open_Create_Replace File.vi"/>
				<Item Name="compatFileDialog.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatFileDialog.vi"/>
				<Item Name="compatOpenFileOperation.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatOpenFileOperation.vi"/>
				<Item Name="compatCalcOffset.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatCalcOffset.vi"/>
				<Item Name="Close File+.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Close File+.vi"/>
				<Item Name="Find First Error.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find First Error.vi"/>
				<Item Name="Merge Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Merge Errors.vi"/>
				<Item Name="Open File+.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/file.llb/Open File+.vi"/>
				<Item Name="NI_AALPro.lvlib" Type="Library" URL="/&lt;vilib&gt;/Analysis/NI_AALPro.lvlib"/>
				<Item Name="Dynamic To Waveform Array.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/transition.llb/Dynamic To Waveform Array.vi"/>
				<Item Name="Waveform Array To Dynamic.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/transition.llb/Waveform Array To Dynamic.vi"/>
				<Item Name="subCurveFitting.vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/CurveFittingBlock.llb/subCurveFitting.vi"/>
				<Item Name="ex_Modify Signals Names.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/ex_Modify Signals Names.vi"/>
				<Item Name="ex_Modify Signal Name.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/ex_Modify Signal Name.vi"/>
				<Item Name="ex_CorrectErrorChain.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/ex_CorrectErrorChain.vi"/>
				<Item Name="NI_MABase.lvlib" Type="Library" URL="/&lt;vilib&gt;/measure/NI_MABase.lvlib"/>
				<Item Name="Get Waveform Time Array.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/WDTOps.llb/Get Waveform Time Array.vi"/>
				<Item Name="WDT Get Waveform Time Array DBL.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/WDTOps.llb/WDT Get Waveform Time Array DBL.vi"/>
				<Item Name="Number of Waveform Samples.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/WDTOps.llb/Number of Waveform Samples.vi"/>
				<Item Name="WDT Number of Waveform Samples DBL.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/WDTOps.llb/WDT Number of Waveform Samples DBL.vi"/>
				<Item Name="WDT Number of Waveform Samples CDB.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/WDTOps.llb/WDT Number of Waveform Samples CDB.vi"/>
				<Item Name="WDT Number of Waveform Samples EXT.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/WDTOps.llb/WDT Number of Waveform Samples EXT.vi"/>
				<Item Name="WDT Number of Waveform Samples I16.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/WDTOps.llb/WDT Number of Waveform Samples I16.vi"/>
				<Item Name="WDT Number of Waveform Samples I32.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/WDTOps.llb/WDT Number of Waveform Samples I32.vi"/>
				<Item Name="WDT Number of Waveform Samples I8.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/WDTOps.llb/WDT Number of Waveform Samples I8.vi"/>
				<Item Name="WDT Number of Waveform Samples SGL.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/WDTOps.llb/WDT Number of Waveform Samples SGL.vi"/>
				<Item Name="DWDT Get Waveform Time Array.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/DWDTOps.llb/DWDT Get Waveform Time Array.vi"/>
				<Item Name="DWDT Digital Size.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/DWDTOps.llb/DWDT Digital Size.vi"/>
				<Item Name="DTbl Digital Size.vi" Type="VI" URL="/&lt;vilib&gt;/Waveform/DTblOps.llb/DTbl Digital Size.vi"/>
				<Item Name="NI_Matrix.lvlib" Type="Library" URL="/&lt;vilib&gt;/Analysis/Matrix/NI_Matrix.lvlib"/>
				<Item Name="NI_Gmath.lvlib" Type="Library" URL="/&lt;vilib&gt;/gmath/NI_Gmath.lvlib"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
			</Item>
			<Item Name="HasMCSRangeChanged1.vi" Type="VI" URL="../HasMCSRangeChanged1.vi"/>
			<Item Name="MCSRange.vi" Type="VI" URL="../MCSRange.vi"/>
			<Item Name="HasSelectionChanged1.vi" Type="VI" URL="../HasSelectionChanged1.vi"/>
			<Item Name="GetSelection1.vi" Type="VI" URL="../GetSelection1.vi"/>
			<Item Name="GetCursor.vi" Type="VI" URL="../GetCursor.vi"/>
			<Item Name="HasMLERangeChanged1.vi" Type="VI" URL="../HasMLERangeChanged1.vi"/>
			<Item Name="MLERange.vi" Type="VI" URL="../MLERange.vi"/>
			<Item Name="CreateNumberedPath.vi" Type="VI" URL="../CreateNumberedPath.vi"/>
			<Item Name="GetUpdatePanel.vi" Type="VI" URL="../GetUpdatePanel.vi"/>
			<Item Name="MCS.vi" Type="VI" URL="../MCS.vi"/>
			<Item Name="BinningTraces.vi" Type="VI" URL="../BinningTraces.vi"/>
			<Item Name="SlidingTraces.vi" Type="VI" URL="../SlidingTraces.vi"/>
			<Item Name="CoincidenceHistogram.vi" Type="VI" URL="../CoincidenceHistogram.vi"/>
			<Item Name="ExportFluoFit.vi" Type="VI" URL="../../Shared/ExportFluoFit.vi"/>
			<Item Name="selectDecayRegion.vi" Type="VI" URL="../selectDecayRegion.vi"/>
			<Item Name="LifetimeHistograms.vi" Type="VI" URL="../LifetimeHistograms.vi"/>
			<Item Name="FCS_Automatic_Filename_sub.vi" Type="VI" URL="../FCS_Automatic_Filename_sub.vi"/>
			<Item Name="FCScluster2SaveArray_sub.vi" Type="VI" URL="../FCScluster2SaveArray_sub.vi"/>
			<Item Name="correlation.vi" Type="VI" URL="../correlation.vi"/>
			<Item Name="Unevenly Sampled Signal Spectrum.vi" Type="VI" URL="../Unevenly Sampled Signal Spectrum.vi"/>
			<Item Name="Std Deviation and Variance.vi" Type="VI" URL="../Std Deviation and Variance.vi"/>
			<Item Name="Power Spectrum.vi" Type="VI" URL="../Power Spectrum.vi"/>
			<Item Name="lvanlys.dll" Type="Document" URL="/C/Program Files (x86)/National Instruments/LabVIEW 2009/resource/lvanlys.dll"/>
			<Item Name="Rawdata.vi" Type="VI" URL="../Rawdata.vi"/>
			<Item Name="Automatic_FileName_sub.vi" Type="VI" URL="../Automatic_FileName_sub.vi"/>
			<Item Name="ParseFIFO_HH.vi" Type="VI" URL="../../Shared/ParseFIFO_HH.vi"/>
			<Item Name="DeleteFinalZeros.vi" Type="VI" URL="../../Shared/DeleteFinalZeros.vi"/>
			<Item Name="ParseFIFO.vi" Type="VI" URL="../ParseFIFO.vi"/>
			<Item Name="Read From I16 File.vi" Type="VI" URL="../Read From I16 File.vi"/>
			<Item Name="Read File+ [I16].vi" Type="VI" URL="../Read File+ [I16].vi"/>
			<Item Name="ClusterFourChannels.vi" Type="VI" URL="../ClusterFourChannels.vi"/>
			<Item Name="BinFIFO.vi" Type="VI" URL="../BinFIFO.vi"/>
			<Item Name="LastElement.vi" Type="VI" URL="../LastElement.vi"/>
			<Item Name="SummarizeFIFOTimes.vi" Type="VI" URL="../SummarizeFIFOTimes.vi"/>
			<Item Name="SetScale.vi" Type="VI" URL="../SetScale.vi"/>
			<Item Name="BinMLE.vi" Type="VI" URL="../BinMLE.vi"/>
			<Item Name="CalcChannelWidth.vi" Type="VI" URL="../CalcChannelWidth.vi"/>
			<Item Name="CalcChannelWidth_HH.vi" Type="VI" URL="../CalcChannelWidth_HH.vi"/>
			<Item Name="BinCoincidence.vi" Type="VI" URL="../BinCoincidence.vi"/>
			<Item Name="SlideFIFO.vi" Type="VI" URL="../SlideFIFO.vi"/>
			<Item Name="SlideLifetime.vi" Type="VI" URL="../SlideLifetime.vi"/>
			<Item Name="SlideCoincidence.vi" Type="VI" URL="../SlideCoincidence.vi"/>
			<Item Name="GetCoincidenceRatio.vi" Type="VI" URL="../GetCoincidenceRatio.vi"/>
			<Item Name="CoincidenceHist.vi" Type="VI" URL="../CoincidenceHist.vi"/>
			<Item Name="DecayCluster2.vi" Type="VI" URL="../DecayCluster2.vi"/>
			<Item Name="FIFOLifetime_HH.vi" Type="VI" URL="../FIFOLifetime_HH.vi"/>
			<Item Name="MLE.vi" Type="VI" URL="../MLE.vi"/>
			<Item Name="on-off-hist.vi" Type="VI" URL="../on-off-hist.vi"/>
			<Item Name="2exp fit cs.vi" Type="VI" URL="../2exp fit cs.vi"/>
			<Item Name="Autocorr_sub.vi" Type="VI" URL="../Autocorr_sub.vi"/>
			<Item Name="FindMaxCountsinTrace_sub.vi" Type="VI" URL="../FindMaxCountsinTrace_sub.vi"/>
			<Item Name="G8uInt16Trace_v1_sub.vi" Type="VI" URL="../G8uInt16Trace_v1_sub.vi"/>
			<Item Name="G8CINuInt32Trace_v2_sub.vi" Type="VI" URL="../G8CINuInt32Trace_v2_sub.vi"/>
			<Item Name="G8uCharTrace_v1_sub.vi" Type="VI" URL="../G8uCharTrace_v1_sub.vi"/>
			<Item Name="diffusion_sub.vi" Type="VI" URL="../diffusion_sub.vi"/>
			<Item Name="Substitute Variables.vi" Type="VI" URL="../Substitute Variables.vi"/>
			<Item Name="String Index.vi" Type="VI" URL="../String Index.vi"/>
			<Item Name="Find String Identifier.vi" Type="VI" URL="../Find String Identifier.vi"/>
			<Item Name="Nonlinear Lev-Mar Fit.vi" Type="VI" URL="../Nonlinear Lev-Mar Fit.vi"/>
			<Item Name="Lev-Mar abx.vi" Type="VI" URL="../Lev-Mar abx.vi"/>
			<Item Name="Target Fnc &amp; Deriv NonLin.vi" Type="VI" URL="../Target Fnc &amp; Deriv NonLin.vi"/>
			<Item Name="partial derivative.vi" Type="VI" URL="../partial derivative.vi"/>
			<Item Name="Lev-Mar xx.vi" Type="VI" URL="../Lev-Mar xx.vi"/>
			<Item Name="get new coefficients.vi" Type="VI" URL="../get new coefficients.vi"/>
			<Item Name="crosscorr_sub2.vi" Type="VI" URL="../crosscorr_sub2.vi"/>
			<Item Name="CC_G2_v2_sub.vi" Type="VI" URL="../CC_G2_v2_sub.vi"/>
			<Item Name="CC_G1st8_v2_sub.vi" Type="VI" URL="../CC_G1st8_v2_sub.vi"/>
			<Item Name="ProcessEarlyGvals_1.vi" Type="VI" URL="../ProcessEarlyGvals_1.vi"/>
			<Item Name="CrossCorr_v1.vi" Type="VI" URL="../CrossCorr_v1.vi"/>
			<Item Name="MacrosToTrace_v5.vi" Type="VI" URL="../MacrosToTrace_v5.vi"/>
			<Item Name="selectByTime.vi" Type="VI" URL="../selectByTime.vi"/>
			<Item Name="F2-alle_sub.vi" Type="VI" URL="../F2-alle_sub.vi"/>
			<Item Name="Crosstalk_sub.vi" Type="VI" URL="../Crosstalk_sub.vi"/>
			<Item Name="Burst7.vi" Type="VI" URL="../Burst7.vi"/>
			<Item Name="F2Value.vi" Type="VI" URL="../F2Value.vi"/>
			<Item Name="BurstRecognition.vi" Type="VI" URL="../BurstRecognition.vi"/>
			<Item Name="autothreshold.vi" Type="VI" URL="../autothreshold.vi"/>
			<Item Name="ThresholdBurst.vi" Type="VI" URL="../ThresholdBurst.vi"/>
			<Item Name="BGCorrection.vi" Type="VI" URL="../BGCorrection.vi"/>
			<Item Name="previousOff.vi" Type="VI" URL="../previousOff.vi"/>
			<Item Name="previousHist.vi" Type="VI" URL="../previousHist.vi"/>
			<Item Name="onCounts.vi" Type="VI" URL="../onCounts.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="FIFOScreenBurst2009 Rev163" Type="EXE">
				<Property Name="App_applicationGUID" Type="Str">{E6BEDBD2-9336-4582-B603-97D21F619B10}</Property>
				<Property Name="App_applicationName" Type="Str">FIFOScreen2009.exe</Property>
				<Property Name="App_companyName" Type="Str">TUM</Property>
				<Property Name="App_fileDescription" Type="Str">FIFOScreenBurst2009 Rev163</Property>
				<Property Name="App_fileVersion.major" Type="Int">1</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{27E19EB3-078D-4774-B35D-CD7C8A8B225C}</Property>
				<Property Name="App_INI_GUID" Type="Str">{89F96509-68B2-4C6E-BBA5-77F06CA46D93}</Property>
				<Property Name="App_internalName" Type="Str">FIFOScreenBurst2009 Rev163</Property>
				<Property Name="App_legalCopyright" Type="Str">Copyright © 2011 TUM</Property>
				<Property Name="App_productName" Type="Str">FIFOScreenBurst2009 Rev163</Property>
				<Property Name="App_winsec.description" Type="Str">http://www.TUM.com</Property>
				<Property Name="Bld_buildSpecName" Type="Str">FIFOScreenBurst2009 Rev163</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Destination[0].destName" Type="Str">FIFOScreen2009.exe</Property>
				<Property Name="Destination[0].path" Type="Path">/F/Repository/Builds/FIFOScreen2009.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">&lt;none&gt;</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">/F/Repository/Builds/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">&lt;none&gt;</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{A4EA0DA2-8183-445C-BA01-1955000817AC}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/FIFOScreenBurst.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
			</Item>
		</Item>
	</Item>
</Project>
