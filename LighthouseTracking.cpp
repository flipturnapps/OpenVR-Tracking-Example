
// The main file for dealing with VR specifically.  See LighthouseTracking.h for descriptions of each function in the class.

#include "LighthouseTracking.h"

// Destructor for the LighthouseTracking object
LighthouseTracking::~LighthouseTracking() 
{
	if (vr_pointer != NULL)
	{
		// VR Shutdown: https://github.com/ValveSoftware/openvr/wiki/API-Documentation#initialization-and-cleanup
		VR_Shutdown(); 
		vr_pointer = NULL;
	}
}

// Constructor for the LighthouseTracking object
LighthouseTracking::LighthouseTracking(InitFlags f) 
{
	flags = f;
	coordsBuf = new char[1024];
	trackBuf = new char[1024];
	rotBuf = new char[1024];
	trackers = new TrackerData[16];

	// Definition of the init error
	EVRInitError eError = VRInitError_None;

	/*
	VR_Init (
	  	arg1: Pointer to EVRInitError type (enum defined in openvr.h)
	  	arg2: Must be of type EVRApplicationType

	  		The type of VR Applicaion.  This example uses the SteamVR instance that is already running.  
	        Because of this, the init function will fail if SteamVR is not already running. 

	        Other EVRApplicationTypes include:
	        	* VRApplication_Scene - "A 3D application that will be drawing an environment.""
	        	* VRApplication_Overlay - "An application that only interacts with overlays or the dashboard.""
	        	* VRApplication_Utility
	*/

	vr_pointer = VR_Init(&eError, VRApplication_Background);

	// If the init failed because of an error
	if (eError != VRInitError_None)
	{
		vr_pointer = NULL;
		printf("Unable to init VR runtime: %s \n", VR_GetVRInitErrorAsEnglishDescription(eError));
		exit(EXIT_FAILURE);
	}

	//If the init didn't fail, init the Cylinder object array
	cylinders = new Cylinder*[MAX_CYLINDERS];
	for(int i = 0 ; i < MAX_CYLINDERS; i++)
	{
		cylinders[i] = new Cylinder();
	}
}


bool LighthouseTracking::RunProcedure() 
{
	// Define a VREvent
	VREvent_t event;
	if(vr_pointer->PollNextEvent(&event, sizeof(event)))
	{
		/*
			ProcessVREvent is a function defined in this module.  It returns false if
			the function determines the type of error to be fatal or signal some kind of quit.
		*/
		if (!ProcessVREvent(event)) 
		{
			// If ProcessVREvent determined that OpenVR quit, print quit message
			printf("\nEVENT--(OpenVR) service quit");
			return false;
		}
	}
	// ParseTrackingFrame() is where the tracking and vibration code starts
	ParseTrackingFrame();
	return true;
}

bool LighthouseTracking::ProcessVREvent(const VREvent_t & event)
{
	char* buf = new char[100];
	bool ret = true;
	switch (event.eventType)
	{
		case VREvent_TrackedDeviceActivated:
			 sprintf(buf, "\nEVENT--(OpenVR) Device : %d attached", event.trackedDeviceIndex);
		break;

		case VREvent_TrackedDeviceDeactivated:
			sprintf(buf, "\nEVENT--(OpenVR) Device : %d detached", event.trackedDeviceIndex);
		break;

		case VREvent_TrackedDeviceUpdated:
			sprintf(buf, "\nEVENT--(OpenVR) Device : %d updated", event.trackedDeviceIndex);
		break;

		case VREvent_DashboardActivated:
			sprintf(buf, "\nEVENT--(OpenVR) Dashboard activated");
		break;

		case VREvent_DashboardDeactivated:
			sprintf(buf, "\nEVENT--(OpenVR) Dashboard deactivated");
		break;

		case VREvent_ChaperoneDataHasChanged:
			sprintf(buf, "\nEVENT--(OpenVR) Chaperone data has changed");
		break;

		case VREvent_ChaperoneSettingsHaveChanged:
			sprintf(buf, "\nEVENT--(OpenVR) Chaperone settings have changed");
		break;

		case VREvent_ChaperoneUniverseHasChanged:
			sprintf(buf, "\nEVENT--(OpenVR) Chaperone universe has changed");
		break;

		case VREvent_ApplicationTransitionStarted:
			sprintf(buf, "\nEVENT--(OpenVR) Application Transition: Transition has started");
		break;

		case VREvent_ApplicationTransitionNewAppStarted:
			sprintf(buf, "\nEVENT--(OpenVR) Application transition: New app has started");
		break;

		case VREvent_Quit:
		{
			sprintf(buf, "\nEVENT--(OpenVR) Received SteamVR Quit (%d%s", VREvent_Quit, ")");
			ret =  false;
		}
		break;

		case VREvent_ProcessQuit:
		{
			sprintf(buf, "\nEVENT--(OpenVR) SteamVR Quit Process (%d%s", VREvent_ProcessQuit, ")");
			ret = false;
		}
		break;

		case VREvent_QuitAborted_UserPrompt:
		{
			sprintf(buf, "\nEVENT--(OpenVR) SteamVR Quit Aborted UserPrompt (%d%s", VREvent_QuitAborted_UserPrompt, ")");
			ret = false;
		}
		break;

		case VREvent_QuitAcknowledged:
		{
			sprintf(buf, "\nEVENT--(OpenVR) SteamVR Quit Acknowledged (%d%s", VREvent_QuitAcknowledged, ")");
			ret = false;
		}
		break;

		case VREvent_TrackedDeviceRoleChanged:
			sprintf(buf, "\nEVENT--(OpenVR) TrackedDeviceRoleChanged: %d", event.trackedDeviceIndex);
		break;

		case VREvent_TrackedDeviceUserInteractionStarted:
			sprintf(buf, "\nEVENT--(OpenVR) TrackedDeviceUserInteractionStarted: %d", event.trackedDeviceIndex);
		break;
		
		default:
			if (event.eventType >= 200 && event.eventType <= 203) //Button events range from 200-203
				dealWithButtonEvent(event);
			else
				sprintf(buf, "\nEVENT--(OpenVR) Event: %d", event.eventType); 
		// Check entire event list starts on line #452: https://github.com/ValveSoftware/openvr/blob/master/headers/openvr.h

	}
	if(flags.printEvents)
		printf("%s",buf);	
	return ret;
}




//This method deals exclusively with button events
void LighthouseTracking::dealWithButtonEvent(VREvent_t event)
{
	int controllerIndex; //The index of the controllers[] array that corresponds with the controller that had a buttonEvent
	for (int i = 0; i < 2; i++) //Iterates across the array of controllers
	{
		ControllerData* pController = &(controllers[i]);
		if(flags.printBEvents && event.trackedDeviceIndex == pController->deviceId) //prints the event data to the terminal
			printf("\nBUTTON-E--index=%d deviceId=%d hand=%d button=%d event=%d",i,pController->deviceId,pController->hand,event.data.controller.button,event.eventType);
		if(pController->deviceId == event.trackedDeviceIndex) //This tests to see if the current controller from the loop is the same from the event
			controllerIndex = i;
	}

	ControllerData* pC = &(controllers[controllerIndex]); //The pointer to the ControllerData struct
	
	if (event.data.controller.button == k_EButton_ApplicationMenu //Test if the ApplicationButton was pressed
		&& event.eventType == VREvent_ButtonUnpress)              //Test if the button is being released (the action happens on release, not press)
	{
		inDrawingMode = !inDrawingMode;
		doRumbleNow = true;
	}
	if(inDrawingMode)
	switch( event.data.controller.button )
	{
		case k_EButton_Grip:  //If it is the grip button that was...
		switch(event.eventType)
		{
			case VREvent_ButtonPress:   // ...pressed...
			if(cpMillis() - gripMillis > 500) // ...and it's been half a second since the grip was last released...
				cylinders[cylinderIndex]->s1[1] = pC->pos.v[1];  //...then set the cylinder's y 1 to the controllers y coordinate.
			break;

			case VREvent_ButtonUnpress: // ...released...
			if(cpMillis() - gripMillis > 500) // ...and it's been half a second since the grip was last released...
				cylinders[cylinderIndex]->s2[1] = pC->pos.v[1];  //...then set the cylinder's y 2 to the controllers y coordinate.
			else                              // ...and it' hasn't been half a second since the grip was last released...
			{
				if(cylinders[cylinderIndex]->s1[1] > pC->pos.v[1])  // ...if the controller's position is **below** the starting position...
					cylinders[cylinderIndex]->s2[1] = -std::numeric_limits<float>::max(); // ...set the cylinder's y 2 to negative infinity.
				else                                                // ...if the controller's position is **above** the starting position...
					cylinders[cylinderIndex]->s2[1] = std::numeric_limits<float>::max();  // ...set the cylinder's y 2 to positive infinity.
			}
				
			cylinders[cylinderIndex]->init();
			gripMillis = cpMillis();
			break;
		}
		break;

		case k_EButton_SteamVR_Trigger:  
		switch(event.eventType)
		{
			case VREvent_ButtonPress:  //If the trigger was pressed...
			cylinders[cylinderIndex]->s1[0] = pC->pos.v[0];  //Set the cylinder's x 1 to the controller's x
			cylinders[cylinderIndex]->s1[2] = pC->pos.v[2];  //Set the cylinder's z 1 to the controller's z
			break;

			case VREvent_ButtonUnpress://If the trigger was released...
			cylinders[cylinderIndex]->s2[0] = pC->pos.v[0];  //Set the cylinder's x 2 to the controller's x
			cylinders[cylinderIndex]->s2[2] = pC->pos.v[2];  //Set the cylinder's z 2 to the controller's z
			cylinders[cylinderIndex]->init();
			break;
		}
		break;

		case k_EButton_SteamVR_Touchpad:
		switch(event.eventType)
		{
			case VREvent_ButtonPress:
			
			break;

			case VREvent_ButtonUnpress://If the touchpad was just pressed
			if(std::abs(pC->padX) > std::abs(pC->padY))       //Tests if the left or right of the pad was pressed
			{
				if (pC->padX < 0 && cylinderIndex != 0)       //If left side of pad was pressed and there is a previous cylinder
					cylinderIndex = cylinderIndex-1;          //Switch index to previous cylinder
				else if (pC->padX > 0 && cylinderIndex < MAX_CYLINDERS)  //If the right side of the pad was pressed 
					cylinderIndex = cylinderIndex+1;          //Switch the index to the next cylinder
				doRumbleNow = true;
			}
			else                         //If the top/bottom of the pad was pressed
			{
				if (pC->padY > 0)        //If the top was pressed               
					doRumbleNow = true;
				else if (pC->padY < 0)   //If the bottom was pressed, reset the current cylinder
					cylinders[cylinderIndex] = new Cylinder();
			}
			break;
		}
		break;
	}
	
}

HmdVector3_t LighthouseTracking::GetPosition(HmdMatrix34_t matrix) 
{
	HmdVector3_t vector;

	vector.v[0] = matrix.m[0][3];
	vector.v[1] = matrix.m[1][3];
	vector.v[2] = matrix.m[2][3];

	return vector;
}

HmdQuaternion_t LighthouseTracking::GetRotation(HmdMatrix34_t matrix)
{
	HmdQuaternion_t q;

	q.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
	q.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
	q.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
	q.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
	q.x = copysign(q.x, matrix.m[2][1] - matrix.m[1][2]);
	q.y = copysign(q.y, matrix.m[0][2] - matrix.m[2][0]);
	q.z = copysign(q.z, matrix.m[1][0] - matrix.m[0][1]);
	return q;
}

HmdVector3_t LighthouseTracking::ProcessRotation(HmdMatrix34_t matrix)
{
	HmdVector3_t ret;
	ret.v[0] = atan2(matrix.m[1][2],matrix.m[2][2]);
	ret.v[1] = atan2(-matrix.m[0][2],
		sqrt(matrix.m[1][2]*matrix.m[1][2]+matrix.m[2][2]*matrix.m[2][2]) );
	ret.v[2] = atan2(matrix.m[0][1],matrix.m[0][0]);

	return ret;
}


void LighthouseTracking::iterateAssignIds()
{
	//Un-assigns the deviceIds and hands of controllers. If they are truely connected, will be re-assigned later in this function
	controllers[0].deviceId = -1;
	controllers[1].deviceId = -1;
	controllers[0].hand = -1;
	controllers[1].hand = -1;

	int numTrackersInitialized = 0;
	int numControllersInitialized = 0;

	for (unsigned int i = 0; i < k_unMaxTrackedDeviceCount; i++)  // Iterates across all of the potential device indicies
	{
		if (!vr_pointer->IsTrackedDeviceConnected(i))
			continue; //Doesn't use the id if the device isn't connected

		//vr_pointer points to the VRSystem that was in init'ed in the constructor.
		ETrackedDeviceClass trackedDeviceClass = vr_pointer->GetTrackedDeviceClass(i);

		//Finding the type of device
		if (trackedDeviceClass == ETrackedDeviceClass::TrackedDeviceClass_HMD)
		{
			hmdDeviceId = i;
			if(flags.printSetIds)
				printf("\nSETID--Assigned hmdDeviceId=%d",hmdDeviceId);
		}
		else if (trackedDeviceClass == ETrackedDeviceClass::TrackedDeviceClass_Controller && numControllersInitialized < 2)
		{
			ControllerData* pC = &(controllers[numControllersInitialized]);

			int sHand = -1;

			ETrackedControllerRole role = vr_pointer->GetControllerRoleForTrackedDeviceIndex(i);
			if (role == TrackedControllerRole_Invalid) //Invalid hand is actually very common, always need to test for invalid hand (lighthouses have lost tracking)
				sHand = 0;
			else if (role == TrackedControllerRole_LeftHand)
				sHand = 1;
			else if (role == TrackedControllerRole_RightHand)
				sHand = 2;
			pC->hand = sHand;
			pC->deviceId = i;


			//Used to get/store property ids for the xy of the pad and the analog reading of the trigger
			for(int x=0; x<k_unControllerStateAxisCount; x++ )
            {
                int prop = vr_pointer->GetInt32TrackedDeviceProperty(pC->deviceId, 
                    (ETrackedDeviceProperty)(Prop_Axis0Type_Int32 + x));

                if( prop==k_eControllerAxis_Trigger )
                    pC->idtrigger = x;
                else if( prop==k_eControllerAxis_TrackPad )
                    pC->idpad = x;
            }			
			if(flags.printSetIds)
				printf("\nSETID--Assigned controllers[%d] .hand=%d .deviceId=%d .idtrigger=%d .idpad=%d",numControllersInitialized,sHand, i , pC->idtrigger, pC->idpad);
			numControllersInitialized++; //Increment this count so that the other controller gets initialized after initializing this one
		}
		else if(trackedDeviceClass == ETrackedDeviceClass::TrackedDeviceClass_GenericTracker)
		{
			TrackerData* pT = &(trackers[numTrackersInitialized]);
			pT->deviceId = i;
			if(flags.printSetIds)
				printf("\nSETID--Assigned tracker[%d] .deviceId=%d",numTrackersInitialized,pT->deviceId);
			numTrackersInitialized++;
		}
			
	}
}

void LighthouseTracking::setHands()
{
	for (int z =0; z < 2; z++)
	{
		ControllerData* pC = &(controllers[z]);
		if (pC->deviceId < 0 || !vr_pointer->IsTrackedDeviceConnected(pC->deviceId))
			continue;
		int sHand = -1;
		//Invalid hand is actually very common, always need to test for invalid hand (lighthouses have lost tracking)
		ETrackedControllerRole role = vr_pointer->GetControllerRoleForTrackedDeviceIndex(pC->deviceId);
		if (role == TrackedControllerRole_Invalid)
			sHand = 0;
		else if (role == TrackedControllerRole_LeftHand)
			sHand = 1;
		else if (role == TrackedControllerRole_RightHand)
			sHand = 2;
		pC->hand = sHand;
	}
}

void LighthouseTracking::ParseTrackingFrame() 
{
	//Runs the iterateAssignIds() method if...
	if(hmdDeviceId < 0 ||                     // HMD id not yet initialized
		controllers[0].deviceId < 0 ||       // One of the controllers not yet initialized
		controllers[1].deviceId < 0 ||
		controllers[0].deviceId == controllers[1].deviceId ||  //Both controllerData structs store the same deviceId
		controllers[0].hand == controllers[1].hand ||          //Both controllerData structs are the same hand
		(cpMillis() / 60000) > minuteCount)                    //It has been a minute since last init time
	{
		minuteCount = (cpMillis() / 60000);
		iterateAssignIds();
	}
	HMDCoords();                            
	ControllerCoords();
	TrackerCoords();
	if(flags.printCoords)
		printf("\nCOORDS-- %s",coordsBuf);
	if(flags.printTrack)
		printf("\nTRACK-- %s",trackBuf);
	if(flags.printRotation)
		printf("\nROT-- %s",rotBuf);
}

void LighthouseTracking::HMDCoords()
{
	if (!vr_pointer->IsTrackedDeviceConnected(hmdDeviceId))
		return;

	//TrackedDevicePose_t struct is a OpenVR struct. See line 180 in the openvr.h header.
	TrackedDevicePose_t trackedDevicePose;
	HmdVector3_t position;
	HmdQuaternion_t rot;
	if (vr_pointer->IsInputFocusCapturedByAnotherProcess())
			printf( "\nINFO--Input Focus by Another Process");
	vr_pointer->GetDeviceToAbsoluteTrackingPose(TrackingUniverseStanding, 0, &trackedDevicePose, 1); 
	position = GetPosition(trackedDevicePose.mDeviceToAbsoluteTracking);
	rot = GetRotation(trackedDevicePose.mDeviceToAbsoluteTracking);
	sprintf(coordsBuf,"HMD %-28.28s", getPoseXYZString(trackedDevicePose,0));
	sprintf(trackBuf,"HMD: %-25.25s %-7.7s " , getEnglishTrackingResultForPose(trackedDevicePose) , getEnglishPoseValidity(trackedDevicePose));
	sprintf(rotBuf,"HMD: qw:%.2f qx:%.2f qy:%.2f qz:%.2f",rot.w,rot.x,rot.y,rot.z);
}

void LighthouseTracking::ControllerCoords()
{
	setHands();
	if(doRumbleNow)
	{
		rumbleMsOffset = cpMillis();
		doRumbleNow = false;
	}

	TrackedDevicePose_t trackedDevicePose;
	VRControllerState_t controllerState;
	HmdQuaternion_t rot;

	//Arrays to contain information about the results of the button state sprintf call 
	//  so that the button state information can be printed all on one line for both controllers
	char** bufs = new char*[2]; 
	bool* isOk = new bool[2];
	
	//Stores the number of times 150ms have elapsed (loops with the % operator because 
	//  the "cylinder count" rumbling starts when indexN is one).
	int indexN = ((cpMillis()-rumbleMsOffset)/150)%(125); 

	//Loops for each ControllerData struct
	for(int i = 0; i < 2; i++)
	{
		isOk[i] = false;
		char* buf = new char[100];
		ControllerData* pC = &(controllers[i]);

		if (pC->deviceId < 0 || 
			!vr_pointer->IsTrackedDeviceConnected(pC->deviceId) || 
			pC->hand </*=  Allow printing coordinates for invalid hand? Yes.*/ 0)
			continue;

		vr_pointer->GetControllerStateWithPose(TrackingUniverseStanding, pC->deviceId, &controllerState, sizeof(controllerState), &trackedDevicePose);
		pC->pos = GetPosition(trackedDevicePose.mDeviceToAbsoluteTracking);	
		rot = GetRotation(trackedDevicePose.mDeviceToAbsoluteTracking);	

		char handString[6];

		if (pC->hand == 1)
			sprintf(handString, "LEFT");
		else if (pC->hand == 2)
			sprintf(handString, "RIGHT");
		else if(pC->hand == 0)
			sprintf(handString, "INVALID");
		
		pC->isValid =trackedDevicePose.bPoseIsValid;

		
		sprintf(coordsBuf,"%s %s: %-28.28s",coordsBuf, handString, getPoseXYZString(trackedDevicePose,pC->hand));
		sprintf(trackBuf,"%s %s: %-25.25s %-7.7s" , trackBuf, handString, getEnglishTrackingResultForPose(trackedDevicePose), getEnglishPoseValidity(trackedDevicePose));
		sprintf(rotBuf,"%s %s qw:%.2f qx:%.2f qy:%.2f qz:%.2f",rotBuf,handString,rot.w,rot.x,rot.y,rot.z);

		int t = pC->idtrigger;
		int p = pC->idpad;

		//This is the call to get analog button data from the controllers
		pC->trigVal = controllerState.rAxis[t].x;
		pC->padX = controllerState.rAxis[p].x;
		pC->padY = controllerState.rAxis[p].y;

		sprintf(buf,"hand=%s handid=%d trigger=%f padx=%f pady=%f", handString, pC->hand , pC->trigVal , pC->padX , pC->padY);
		bufs[i] = buf;
		isOk[i] = true;

		//The following block controlls the rumbling of the controllers
		if(!inDrawingMode) //Will iterate across all cylinders if in sensing mode
		for(int x = 0; x < MAX_CYLINDERS; x++)
		{
			Cylinder* currCy = cylinders[x];
			if(currCy->hasInit && 
				currCy->isInside(pC->pos.v[0],pC->pos.v[1],pC->pos.v[2]))
				vr_pointer->TriggerHapticPulse(pC->deviceId,pC->idpad,500); //Vibrates if the controller is colliding with the cylinder bounds
		}
		if (inDrawingMode && indexN % 3 == 0 && indexN < (cylinderIndex+1)*3) //Vibrates the current cylinderIndex every thirty seconds or so
			vr_pointer->TriggerHapticPulse(pC->deviceId,pC->idpad,300);	      //  see the definition of indexN above before the for loop
	}
	
	if(flags.printAnalog && isOk[0] == true)
	{
		printf("\nANALOG-- %s", bufs[0]);
		if(isOk[1] == true)
		{
			printf("  %s", bufs[1]);
		}
	}
}

void LighthouseTracking::TrackerCoords()
{
	TrackedDevicePose_t trackedDevicePose;
	VRControllerState_t controllerState;
	HmdQuaternion_t rot;

	for(int i = 0; i < 16; i++)
	{
		TrackerData* pT = &(trackers[i]);

		if (pT->deviceId < 0 || 
			!vr_pointer->IsTrackedDeviceConnected(pT->deviceId) ) 
			continue;

		vr_pointer->GetControllerStateWithPose(TrackingUniverseStanding, pT->deviceId, &controllerState, sizeof(controllerState), &trackedDevicePose);
		pT->pos = GetPosition(trackedDevicePose.mDeviceToAbsoluteTracking);	
		rot = GetRotation(trackedDevicePose.mDeviceToAbsoluteTracking);
		pT->isValid =trackedDevicePose.bPoseIsValid;

		sprintf(coordsBuf,"%s T%d: %-28.28s",coordsBuf, i, getPoseXYZString(trackedDevicePose,0));
		sprintf(trackBuf,"%s T%d: %-25.25s %-7.7s" , trackBuf, i, getEnglishTrackingResultForPose(trackedDevicePose), getEnglishPoseValidity(trackedDevicePose));
		sprintf(rotBuf,"%s T%d: qw:%.2f qx:%.2f qy:%.2f qz:%.2f",rotBuf,i,rot.w,rot.x,rot.y,rot.z);
	}
}

char* LighthouseTracking::getEnglishTrackingResultForPose(TrackedDevicePose_t pose)
{
	char* buf = new char[50];
	switch (pose.eTrackingResult) 
	{
		case vr::ETrackingResult::TrackingResult_Uninitialized:
				sprintf(buf, "Invalid tracking result");
				break;
		case vr::ETrackingResult::TrackingResult_Calibrating_InProgress:
				sprintf(buf, "Calibrating in progress");
				break;
		case vr::ETrackingResult::TrackingResult_Calibrating_OutOfRange:
				sprintf(buf, "Calibrating Out of range");
				break;
		case vr::ETrackingResult::TrackingResult_Running_OK:
				sprintf(buf, "Running OK");
				break;
		case vr::ETrackingResult::TrackingResult_Running_OutOfRange:
				sprintf(buf, "WARNING: Running Out of Range");
				break;
		default:
				sprintf(buf, "Default");
				break;
	}
	return buf;
}

char* LighthouseTracking::getEnglishPoseValidity(TrackedDevicePose_t pose)
{
	char* buf = new char[50];
	if(pose.bPoseIsValid)
		sprintf(buf, "Valid");
	else
		sprintf(buf, "Invalid");
	return buf;
}

char* LighthouseTracking::getPoseXYZString(TrackedDevicePose_t pose, int hand)
{
	HmdVector3_t pos = ProcessRotation(pose.mDeviceToAbsoluteTracking);
	char* cB = new char[50];
	if(pose.bPoseIsValid)
		sprintf(cB, "x:%.3f y:%.3f z:%.3f",pos.v[0], pos.v[1], pos.v[2]);
	else
		sprintf(cB, "            INVALID");	
	if(flags.pipeCoords)
		for(int i = 0; i < 3; i++)
			if(pose.bPoseIsValid)
				printf("%.5f\n",pos.v[i]);
			else
				printf("invalid\n",pos.v[i]);
	return cB;
}

