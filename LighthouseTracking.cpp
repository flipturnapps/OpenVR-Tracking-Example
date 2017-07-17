#include "LighthouseTracking.h"
#include <stdio.h>



// Destructor
LighthouseTracking::~LighthouseTracking() 
{
	if (vr_pointer != NULL)
	{
		VR_Shutdown();
		vr_pointer = NULL;
	}
}

// Constructor
LighthouseTracking::LighthouseTracking() 
{
	// Definition of EVRInitError local variable
	EVRInitError eError = VRInitError_None;

	/*
	VR_Init (
	  	arg1: Pointer to EVRInitError type (I think its an enum)
	  	arg2: Must be of type EVRApplicationType

	  		The type of VR Applicaion.  This example uses the SteamVR instance that is already running.  
	        Because of this, the init function will fail if SteamVR is not already running. 

	        Other EVRApplicationTypes include:
	        	* VRApplication_Scene - "A 3D application that will be drawing an environment.""
	        	* VRApplication_Overlay - "An application that only interacts with overlays or the dashboard.""
	        	* VRApplication_Utility
	*/

	vr_pointer = VR_Init(&eError, VRApplication_Background);

	// If the init failed because of the error
	if (eError != VRInitError_None)
	{
		vr_pointer = NULL;
		printf("Unable to init VR runtime: %s \n", VR_GetVRInitErrorAsEnglishDescription(eError));
		exit(EXIT_FAILURE);
	}
}

/*
	defined fuction RunProcedure()

	* Loop-listen for events then parses them (e.g. prints the to user)
	* Returns true if success or false if openvr has quit
	* Should be called by main.cpp 
*/
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
	ParseTrackingFrame();
	return true;
}

/*
	defined fuction ProcessVREvent()

	* Takes a single vr:VREvent_t and processes the result
	* Returns false if OpenVR has quit, otherwise returns true
*/
bool LighthouseTracking::ProcessVREvent(const VREvent_t & event)
{
	switch (event.eventType)
	{
		case VREvent_TrackedDeviceActivated:
			 printf("\nEVENT--(OpenVR) Device : %d attached", event.trackedDeviceIndex);
		break;

		case VREvent_TrackedDeviceDeactivated:
			printf("\nEVENT--(OpenVR) Device : %d detached", event.trackedDeviceIndex);
		break;

		case VREvent_TrackedDeviceUpdated:
			printf("\nEVENT--(OpenVR) Device : %d updated", event.trackedDeviceIndex);
		break;

		case VREvent_DashboardActivated:
			printf("\nEVENT--(OpenVR) Dashboard activated");
		break;

		case VREvent_DashboardDeactivated:
			printf("\nEVENT--(OpenVR) Dashboard deactivated");
		break;

		case VREvent_ChaperoneDataHasChanged:
			printf("\nEVENT--(OpenVR) Chaperone data has changed");
		break;

		case VREvent_ChaperoneSettingsHaveChanged:
			printf("\nEVENT--(OpenVR) Chaperone settings have changed");
		break;

		case VREvent_ChaperoneUniverseHasChanged:
			printf("\nEVENT--(OpenVR) Chaperone universe has changed");
		break;

		case VREvent_ApplicationTransitionStarted:
			printf("\nEVENT--(OpenVR) Application Transition: Transition has started");
		break;

		case VREvent_ApplicationTransitionNewAppStarted:
			printf("\nEVENT--(OpenVR) Application transition: New app has started");
		break;

		case VREvent_Quit:
		{
			printf("\nEVENT--(OpenVR) Received SteamVR Quit (%d%s", VREvent_Quit, ")");
			return false;
		}
		break;

		case VREvent_ProcessQuit:
		{
			printf("\nEVENT--(OpenVR) SteamVR Quit Process (%d%s", VREvent_ProcessQuit, ")");
			return false;
		}
		break;

		case VREvent_QuitAborted_UserPrompt:
		{
			printf("\nEVENT--(OpenVR) SteamVR Quit Aborted UserPrompt (%d%s", VREvent_QuitAborted_UserPrompt, ")");
			return false;
		}
		break;

		case VREvent_QuitAcknowledged:
		{
			printf("\nEVENT--(OpenVR) SteamVR Quit Acknowledged (%d%s", VREvent_QuitAcknowledged, ")");
			return false;
		}
		break;

		case VREvent_TrackedDeviceRoleChanged:
			printf("\nEVENT--(OpenVR) TrackedDeviceRoleChanged: %d", event.trackedDeviceIndex);
		break;

		case VREvent_TrackedDeviceUserInteractionStarted:
			printf("\nEVENT--(OpenVR) TrackedDeviceUserInteractionStarted: %d", event.trackedDeviceIndex);
		break;
		
		default:
			if (event.eventType >= 200 && event.eventType <= 203)
				dealWithButtonEvent(event);
			else
				printf("\nEVENT--(OpenVR) Event: %d", event.eventType);
	}

	return true;
}

//This method deals exclusively with button events
void LighthouseTracking::dealWithButtonEvent(VREvent_t event)
{
	int controllerIndex;
	for (int i = 0; i < 2; i++)
	{
		ControllerData* pController = &(controllers[i]);
		if(event.trackedDeviceIndex == pController->deviceId) //prints the event data to the terminal
			printf("\nBUTTON-E--index=%d deviceId=%d hand=%d button=%d event=%d",i,pController->deviceId,pController->hand,event.data.controller.button,event.eventType);
		if(pController->deviceId == event.trackedDeviceIndex)
			controllerIndex = i;
	}
	ControllerData* pC = &(controllers[controllerIndex]);
	ButtonData* pBD;
	
	switch( event.data.controller.button )
            {
            case k_EButton_ApplicationMenu:
              	pBD = &(pC->b_AppMenu);
                break;

            case k_EButton_Grip:
                pBD = &(pC->b_Grip);
                break;

            case k_EButton_SteamVR_Trigger:
                pBD = &(pC->b_Trigger);
                break;

            case k_EButton_SteamVR_Touchpad:
                pBD = &(pC->b_Pad);
                break;
            }
    switch(event.eventType)
            {
            case VREvent_ButtonPress:
               	pBD->pressed = true;           
                break;

            case VREvent_ButtonUnpress:
                pBD->pressed = false;         
                break;

            case VREvent_ButtonTouch:
                pBD->touched = true;
                break;

            case VREvent_ButtonUntouch:
                pBD->touched = false;
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

//This method iterates across the maximum number of devices that can be attatched to the SteamVR system
// and finds which ids correlate to the HMD and controllers;
void LighthouseTracking::iterateAssignIds()
{
	initPassCount = 0;
	for (unsigned int i = 0; i < k_unMaxTrackedDeviceCount; i++)
	{
		if (!vr_pointer->IsTrackedDeviceConnected(i))
			continue; //Doesn't use the id if the device isn't connected


		ETrackedDeviceClass trackedDeviceClass = vr_pointer->GetTrackedDeviceClass(i);

		//Finding the type of device
		if (trackedDeviceClass == ETrackedDeviceClass::TrackedDeviceClass_HMD)
		{
			hmdDeviceId = i;
			printf("\nSETID--Assigned hmdDeviceId=%d",hmdDeviceId);
		}
		else if (trackedDeviceClass == ETrackedDeviceClass::TrackedDeviceClass_Controller)
		{
			int initIndex = controllerInitCount % 2;
			ControllerData* pController = &(controllers[initIndex]);
			int sHand = -1;

			ETrackedControllerRole role = vr_pointer->GetControllerRoleForTrackedDeviceIndex(i);
			if (role == TrackedControllerRole_Invalid)
				sHand = 0;
			else if (role == TrackedControllerRole_LeftHand)
				sHand = 1;
			else if (role == TrackedControllerRole_RightHand)
				sHand = 2;
			pController->hand = sHand;
			pController->deviceId = i;



			for(int x=0; x<k_unControllerStateAxisCount; x++ )
            {
                int prop = vr_pointer->GetInt32TrackedDeviceProperty(pController->deviceId, 
                    (ETrackedDeviceProperty)(Prop_Axis0Type_Int32 + x));

                if( prop==k_eControllerAxis_Trigger )
                    pController->idtrigger = x;
                else if( prop==k_eControllerAxis_TrackPad )
                    pController->idpad = x;
            }


			controllerInitCount++;
			printf("\nSETID--Assigned controllers[%d] .hand=%d .deviceId=%d .idtrigger=%d .idpad=%d",initIndex,sHand, i , pController->idtrigger, (*pController).idpad);
		}
			
	}
}

void LighthouseTracking::ParseTrackingFrame() 
{
	/*
		This for loop will iterate over all of the tracked devices.
		* deviceId is the locaL variable holding the index.
	*/

	//Runs the iterateAssignIds() method if not every device has an id, or if it has been 5000 iterations
	if(hmdDeviceId < 0 ||
		controllers[0].deviceId < 0 ||
		controllers[1].deviceId < 0 ||
		controllers[0].deviceId == controllers[1].deviceId || 
		controllers[0].hand == controllers[1].hand ||
		initPassCount > 5000)
		iterateAssignIds();
	else
		initPassCount++;

	HMDCoords();
	ControllerCoords();
}

void LighthouseTracking::HMDCoords()
{
	if (!vr_pointer->IsTrackedDeviceConnected(hmdDeviceId))
		return;
	TrackedDevicePose_t trackedDevicePose;
	HmdVector3_t position;
	if (vr_pointer->IsInputFocusCapturedByAnotherProcess())
			printf( "\nINFO--Input Focus by Another Process");

	vr_pointer->GetDeviceToAbsoluteTrackingPose(TrackingUniverseStanding, 0, &trackedDevicePose, 1);
	position = GetPosition(trackedDevicePose.mDeviceToAbsoluteTracking);
	printf("\nCOORDS-- HMD x: %.3f y: %.3f z: %.3f", position.v[0], position.v[1], position.v[2]);
}

void LighthouseTracking::ControllerCoords()
{
	TrackedDevicePose_t trackedDevicePose;
	VRControllerState_t controllerState;

	char** bufs = new char*[2];
	bool* isOk = new bool[2];
	for(int i = 0; i < 2; i++)
	{
		isOk[i] = false;
		char* buf = new char[100];
		ControllerData* controller = &(controllers[i]);

		if (controller->deviceId < 0 || 
			!vr_pointer->IsTrackedDeviceConnected(controller->deviceId) || 
			controller->hand <= 0)
			continue;

		vr_pointer->GetControllerStateWithPose(TrackingUniverseStanding, controller->deviceId, &controllerState, sizeof(controllerState), &trackedDevicePose);
		controller->pos = GetPosition(trackedDevicePose.mDeviceToAbsoluteTracking);	
		
		char handString[6];

		if (controller->hand == 1)
			sprintf(handString, "LEFT");
		else if (controller->hand == 2)
			sprintf(handString, "RIGHT");

		printf(" %s x: %.3f y: %.3f z: %.3f", handString, controller->pos.v[0], controller->pos.v[1], controller->pos.v[2]);	

		int t = controller->idtrigger;
		int p = controller->idpad;

		sprintf(buf,"hand=%s handid=%d trigger=%f padx=%f pady=%f", handString, controller->hand , controllerState.rAxis[t].x,controllerState.rAxis[p].x,controllerState.rAxis[p].y);
		bufs[i] = buf;
		isOk[i] = true;
	}
	
	if(isOk[0] == true)
	{
		printf("\nBUTTON-S-- %s",( (bufs[0]) ) );
		if(isOk[1] == true)
		{
			printf("  %s",( (bufs[1]) ) );
		}
	}
	
}
