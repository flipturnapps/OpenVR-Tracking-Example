// The main file for dealing with VR specifically.  See LighthouseTracking.h for descriptions of each function in the class.

#include "LighthouseTracking.h"
#include <stdio.h>
#include "cylinder.h"
#include "cpTime.h"

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
LighthouseTracking::LighthouseTracking() 
{
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
	cylinders = new Cylinder*[10];
	for(int i = 0 ; i < 10; i++)
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
			if (event.eventType >= 200 && event.eventType <= 203) //Button events range from 200-203
				dealWithButtonEvent(event);
			else
				printf("\nEVENT--(OpenVR) Event: %d", event.eventType); 
		// Check entire event list starts on line #452: https://github.com/ValveSoftware/openvr/blob/master/headers/openvr.h

	}
	return true;
}

//Stores the number of ms elapsed when the grip was released.
long gripMillis;


//This method deals exclusively with button events
void LighthouseTracking::dealWithButtonEvent(VREvent_t event)
{
	int controllerIndex; //The index of the controllers[] array that corresponds with the controller that had a buttonEvent
	for (int i = 0; i < 2; i++) //Iterates across the array of controllers
	{
		ControllerData* pController = &(controllers[i]);
		if(event.trackedDeviceIndex == pController->deviceId) //prints the event data to the terminal
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
				else if (pC->padX > 0 && cylinderIndex < 10)  //If the right side of the pad was pressed 
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


void LighthouseTracking::iterateAssignIds()
{
	initPassCount = 0; // initPassCount is a field which counts how many frames have gone by without running this method
	                   //   if this method is currently executing, then zero frames have gone by since the last time it ran
	
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
			printf("\nSETID--Assigned hmdDeviceId=%d",hmdDeviceId);
		}
		else if (trackedDeviceClass == ETrackedDeviceClass::TrackedDeviceClass_Controller)
		{
			//Uses the modulus operator so that the same controller in the array doesn't keep being initialized.
			//  Should alternate which index is used each time this block gets run.
			int initIndex = controllerInitCount % 2; 
			ControllerData* pC = &(controllers[initIndex]);

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



			for(int x=0; x<k_unControllerStateAxisCount; x++ )
            {
                int prop = vr_pointer->GetInt32TrackedDeviceProperty(pController->deviceId, 
                    (ETrackedDeviceProperty)(Prop_Axis0Type_Int32 + x));

                if( prop==k_eControllerAxis_Trigger )
                    pC->idtrigger = x;
                else if( prop==k_eControllerAxis_TrackPad )
                    pC->idpad = x;
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

		controller->trigVal = controllerState.rAxis[t].x;
		controller->padX = controllerState.rAxis[p].x;
		controller->padY = controllerState.rAxis[p].y;

		sprintf(buf,"hand=%s handid=%d trigger=%f padx=%f pady=%f", handString, controller->hand , controller->trigVal , controller->padX , controller->padY);
		bufs[i] = buf;
		isOk[i] = true;
	}
	
	if(false && isOk[0] == true)
	{
		printf("\nBUTTON-S-- %s",( (bufs[0]) ) );
		if(isOk[1] == true)
		{
			printf("  %s",( (bufs[1]) ) );
		}
	}

	if(doRumbleNow)
	{
		rumbleMsOffset = cpMillis();
		doRumbleNow = false;
	}

	int indexN = ((cpMillis()-rumbleMsOffset)/150)%(125);
	for (int i = 0; i < 2; i++)
	{

		ControllerData c = controllers[i];

		if(!inDrawingMode)
		for(int x = 0; x < 10 ; x++)
		{
			Cylinder*  currCy = cylinders[x];
			if(!inDrawingMode && currCy->hasInit && currCy->isInside(c.pos.v[0],c.pos.v[1],c.pos.v[2] ))
				vr_pointer->TriggerHapticPulse(c.deviceId,c.idpad,400);
		}


		if (inDrawingMode && indexN % 3 == 0 && indexN < (cylinderIndex+1)*3)
			vr_pointer->TriggerHapticPulse(c.deviceId,c.idpad,300);	
	}	


	
   // printf("\n%lld",cpMillis());
}
