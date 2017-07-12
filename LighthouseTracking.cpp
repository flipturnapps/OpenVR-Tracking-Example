//
// HTC Vive Lighthouse Tracking Example
// By Peter Thor 2016
//
// Shows how to extract basic tracking data
//

#include "LighthouseTracking.h"
#include <stdio.h>


// Destructor
LighthouseTracking::~LighthouseTracking() {
	if (m_pHMD != NULL)
	{
		vr::VR_Shutdown();
		m_pHMD = NULL;
	}
}

// Constructor
LighthouseTracking::LighthouseTracking() {
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Background);

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		printf("Unable to init VR runtime: %s \n", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		exit(EXIT_FAILURE);
	}
}

/*
* Loop-listen for events then parses them (e.g. prints the to user)
* Returns true if success or false if openvr has quit
*/
bool LighthouseTracking::RunProcedure(bool bWaitForEvents) 
{
	//printf("\nTEST--RunProcedure wait for events: %s", bWaitForEvents ? "true" : "false");
	// Either A) wait for events, such as hand controller button press, before parsing...
	if (bWaitForEvents) {
		// Process VREvent
		vr::VREvent_t event;
		while (m_pHMD->PollNextEvent(&event, sizeof(event)))
		{
			// Process event
			if (!ProcessVREvent(event)) {
				/* info */ printf("\n(OpenVR) service quit");
				//return false;
			}

			ParseTrackingFrame();
		}
	}
	else {
		// ... or B) continous parsint of tracking data irrespective of events
		//std::cout << std::endl << "Parsing next frame...";

		ParseTrackingFrame();
	}
	
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Processes a single VR event
//-----------------------------------------------------------------------------

bool LighthouseTracking::ProcessVREvent(const vr::VREvent_t & event)
{
	//printf("\nTEST--Process vr event");
	switch (event.eventType)
	{
		case vr::VREvent_TrackedDeviceActivated:
		{
			//SetupRenderModelForTrackedDevice(event.trackedDeviceIndex);
			/* info */ printf("\n(OpenVR) Device : %d attached", event.trackedDeviceIndex);
		}
		break;

		case vr::VREvent_TrackedDeviceDeactivated:
		{
			/* info */ printf("\n(OpenVR) Device : %d detached", event.trackedDeviceIndex);
		}
		break;

		case vr::VREvent_TrackedDeviceUpdated:
		{
			/* info */ printf("\n(OpenVR) Device : %d updated", event.trackedDeviceIndex);
		}
		break;

		case (vr::VREvent_DashboardActivated) :
		{
			/* info */ printf("\n(OpenVR) Dashboard activated");
		}
		break;

		case (vr::VREvent_DashboardDeactivated) :
		{
			/* info */ printf("\n(OpenVR) Dashboard deactivated");
		}
		break;

		case (vr::VREvent_ChaperoneDataHasChanged) :
		{
			/* info */ printf("\n(OpenVR) Chaperone data has changed");
		}
		break;

		case (vr::VREvent_ChaperoneSettingsHaveChanged) :
		{
			/* info */ printf("\n(OpenVR) Chaperone settings have changed");
		}
		break;

		case (vr::VREvent_ChaperoneUniverseHasChanged) :
		{
			/* info */ printf("\n(OpenVR) Chaperone universe has changed");
		}
		break;

		case (vr::VREvent_ApplicationTransitionStarted) :
		{
			/* info */ printf("\n(OpenVR) Application Transition: Transition has started");
		}
		break;

		case (vr::VREvent_ApplicationTransitionNewAppStarted) :
		{
			/* info */ printf("\n(OpenVR) Application transition: New app has started");
		}
		break;

		case (vr::VREvent_Quit) :
		{

			/* info */ printf("\n(OpenVR) Received SteamVR Quit (%d", vr::VREvent_Quit, ")");
			return false;
		}
		break;

		case (vr::VREvent_ProcessQuit) :
		{
			/* info */ printf("\n(OpenVR) SteamVR Quit Process (%d", vr::VREvent_ProcessQuit, ")");
			return false;
		}
		break;

		case (vr::VREvent_QuitAborted_UserPrompt) :
		{
			/* info */ printf("\n(OpenVR) SteamVR Quit Aborted UserPrompt (%d", vr::VREvent_QuitAborted_UserPrompt, ")");
			return false;
		}
		break;

		case (vr::VREvent_QuitAcknowledged) :
		{
			/* info */ printf("\n(OpenVR) SteamVR Quit Acknowledged (%d", vr::VREvent_QuitAcknowledged, ")");
			return false;
		}
		break;

		case (vr::VREvent_TrackedDeviceRoleChanged) :
		{
			/* info */ printf("\n(OpenVR) TrackedDeviceRoleChanged: %d", event.trackedDeviceIndex);
			break;
		}

		case (vr::VREvent_TrackedDeviceUserInteractionStarted) :
		{
			/* info */ printf("\n(OpenVR) TrackedDeviceUserInteractionStarted: %d", event.trackedDeviceIndex);
			break;
		}

		default:
			if (event.eventType >= 200 && event.eventType <= 203)
				/* info */ printf("\n(OpenVR) Buttony Thing #: %d", event.eventType);
			else
				/* info */ printf("\n(OpenVR) Event: %d", event.eventType);
			break;
	}

	return true;
}


// Get the quaternion representing the rotation
vr::HmdQuaternion_t LighthouseTracking::GetRotation(vr::HmdMatrix34_t matrix) {
	vr::HmdQuaternion_t q;

	q.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
	q.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
	q.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
	q.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
	q.x = copysign(q.x, matrix.m[2][1] - matrix.m[1][2]);
	q.y = copysign(q.y, matrix.m[0][2] - matrix.m[2][0]);
	q.z = copysign(q.z, matrix.m[1][0] - matrix.m[0][1]);
	return q;
}

// Get the vector representing the position
vr::HmdVector3_t LighthouseTracking::GetPosition(vr::HmdMatrix34_t matrix) {
	vr::HmdVector3_t vector;

	vector.v[0] = matrix.m[0][3];
	vector.v[1] = matrix.m[1][3];
	vector.v[2] = matrix.m[2][3];

	return vector;
}

/*
* Parse a Frame with data from the tracking system
*
* Handy reference:
* https://github.com/TomorrowTodayLabs/NewtonVR/blob/master/Assets/SteamVR/Scripts/SteamVR_Utils.cs
*
* Also:
* Open VR Convention (same as OpenGL)
* right-handed system
* +y is up
* +x is to the right
* -z is going away from you
* http://www.3dgep.com/understanding-the-view-matrix/
*
*/

/*
void sprintf_s(char* a, int b, char* c,int d)
{
	snprintf(a,b,c,d);
}
*/

void LighthouseTracking::ParseTrackingFrame() {
	
	static FILE *out_file_x;
	if(!out_file_x)
		out_file_x = fopen("r-x.txt", "w");
	static FILE *out_file_y;
	if(!out_file_y)
		out_file_y = fopen("r-y.txt", "w");
	static FILE *out_file_z;
	if(!out_file_z)
		out_file_z = fopen("r-z.txt", "w");

	// Process SteamVR device states
	for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
	{
		// if not connected just skip the rest of the routine
		if (!m_pHMD->IsTrackedDeviceConnected(unDevice))
			continue;

		vr::TrackedDevicePose_t trackedDevicePose;
		vr::TrackedDevicePose_t *devicePose = &trackedDevicePose;

		vr::VRControllerState_t controllerState;
		vr::VRControllerState_t *ontrollerState_ptr = &controllerState;

		vr::HmdVector3_t position;
		vr::HmdQuaternion_t quaternion;

		if (vr::VRSystem()->IsInputFocusCapturedByAnotherProcess()) {
			/* info */ printf( "\nInput Focus by Another Process");
		}

		bool bPoseValid = trackedDevicePose.bPoseIsValid;
		vr::HmdVector3_t vVel;
		vr::HmdVector3_t vAngVel;
		vr::ETrackingResult eTrackingResult;

		// Get what type of device it is and work with its data
		vr::ETrackedDeviceClass trackedDeviceClass = vr::VRSystem()->GetTrackedDeviceClass(unDevice);
		switch (trackedDeviceClass) {
		case vr::ETrackedDeviceClass::TrackedDeviceClass_HMD:
			// print stuff for the HMD here, see controller stuff in next case block

			// get pose relative to the safe bounds defined by the user
			vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, &trackedDevicePose, 1);

			// get the position and rotation
			position = GetPosition(devicePose->mDeviceToAbsoluteTracking);
			quaternion = GetRotation(devicePose->mDeviceToAbsoluteTracking);

			// get some data
			vVel = trackedDevicePose.vVelocity;
			vAngVel = trackedDevicePose.vAngularVelocity;
			eTrackingResult = trackedDevicePose.eTrackingResult;
			bPoseValid = trackedDevicePose.bPoseIsValid;


			printf("\nCOORDS-- HMD x: %.3f y: %.3f z: %.3f", position.v[0], position.v[1], position.v[2]);
			//printf("qw: %.2f qx: %.2f qy: %.2f qz: %.2f", quaternion.w, quaternion.x, quaternion.y, quaternion.z);

			
			// and print some more info to the user about the state of the device/pose
			switch (eTrackingResult) {
			case vr::ETrackingResult::TrackingResult_Uninitialized:
				// omitted-info printf( "Invalid tracking result\n");
				break;
			case vr::ETrackingResult::TrackingResult_Calibrating_InProgress:
				// omitted-info printf( "Calibrating in progress\n");
				break;
			case vr::ETrackingResult::TrackingResult_Calibrating_OutOfRange:
				// omitted-info printf( "Calibrating Out of range\n");
				break;
			case vr::ETrackingResult::TrackingResult_Running_OK:
				// omitted-info printf( "Running OK\n");
				break;
			case vr::ETrackingResult::TrackingResult_Running_OutOfRange:
				// omitted-info printf( "WARNING: Running Out of Range\n");
				break;
			default:
				// omitted-info printf( "Default\n");
				break;
			}

			/*
			// print if the pose is valid or not
			if (bPoseValid)
				// omitted-info printf( "Valid pose\n");
			else
				// omitted-info printf( "Invalid pose\n");
			*/
			break;


#pragma region Controller



		case vr::ETrackedDeviceClass::TrackedDeviceClass_Controller:
			// Simliar to the HMD case block above, please adapt as you like
			// to get away with code duplication and general confusion

			vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, unDevice, &controllerState, sizeof(controllerState), &trackedDevicePose);

			position = GetPosition(devicePose->mDeviceToAbsoluteTracking);
			quaternion = GetRotation(devicePose->mDeviceToAbsoluteTracking);

			vVel = trackedDevicePose.vVelocity;
			vAngVel = trackedDevicePose.vAngularVelocity;
			eTrackingResult = trackedDevicePose.eTrackingResult;
			bPoseValid = trackedDevicePose.bPoseIsValid;

			switch (vr::VRSystem()->GetControllerRoleForTrackedDeviceIndex(unDevice)) {
			case vr::TrackedControllerRole_Invalid:
				// invalid hand...
				break;

			
			case vr::TrackedControllerRole_LeftHand:

				printf(" LEFT x: %.3f y: %.3f z: %.3f", position.v[0], position.v[1], position.v[2]);
				//printf(" qw: %.2f qx: %.2f qy: %.2f qz: %.2f", quaternion.w, quaternion.x, quaternion.y, quaternion.z);

				switch (eTrackingResult) {
				case vr::ETrackingResult::TrackingResult_Uninitialized:
					// omitted-info printf( "Invalid tracking result\n");
					break;
				case vr::ETrackingResult::TrackingResult_Calibrating_InProgress:
					// omitted-info printf( "Calibrating in progress\n");
					break;
				case vr::ETrackingResult::TrackingResult_Calibrating_OutOfRange:
					// omitted-info printf( "Calibrating Out of range\n");
					break;
				case vr::ETrackingResult::TrackingResult_Running_OK:
					// omitted-info printf( "Running OK\n");
					break;
				case vr::ETrackingResult::TrackingResult_Running_OutOfRange:
					// omitted-info printf( "WARNING: Running Out of Range\n");
					break;
				default:
					// omitted-info printf( "Default\n");
					break;
				}

				/*
				if (bPoseValid)
					// omitted-info printf( "Valid pose\n");
				else
					// omitted-info printf( "Invalid pose\n");
				*/
				break;
				
			case vr::TrackedControllerRole_RightHand:
				fprintf(out_file_x, "%.3f\n", position.v[0]);
				fprintf(out_file_y, "%.3f\n", position.v[1]);
				fprintf(out_file_z, "%.3f\n", position.v[2]);

				printf(" RIGHT x: %.3f y: %.3f z: %.3f", position.v[0], position.v[1], position.v[2]);
				//printf(" qw: %.2f qx: %.2f qy: %.2f qz: %.2f", quaternion.w, quaternion.x, quaternion.y, quaternion.z);
				break;

			case vr::TrackedDeviceClass_TrackingReference:
				// incomplete code, only here for switch reference
				// omitted-info printf( "Camera / Base Station");
				break;
			}

			break;
#pragma endregion
		case vr::ETrackedDeviceClass::TrackedDeviceClass_GenericTracker:
			//tracking universe, device index, controller state object, controller pose object
			vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, unDevice, &controllerState, sizeof(controllerState), &trackedDevicePose);
			position = GetPosition(devicePose->mDeviceToAbsoluteTracking);
			quaternion = GetRotation(devicePose->mDeviceToAbsoluteTracking);

			vVel = trackedDevicePose.vVelocity;
			vAngVel = trackedDevicePose.vAngularVelocity;
			eTrackingResult = trackedDevicePose.eTrackingResult;
			bPoseValid = trackedDevicePose.bPoseIsValid;

			//std::cout << std::endl << "Vive Tracker Connected" << std::endl;
			//std::cout << position.v[0] << ", " << position.v[1] << ", " << position.v[2] << std::endl;
			

			printf(" TRACKER x: %.3f y: %.3f z: %.3f", position.v[0], position.v[1], position.v[2]);
			//printf(" qw: %.2f qx: %.2f qy: %.2f qz: %.2f", quaternion.w, quaternion.x, quaternion.y, quaternion.z);

			switch (eTrackingResult) {
			case vr::ETrackingResult::TrackingResult_Uninitialized:
				//sprintf_s(puf, sizeof(puf), "Invalid tracking result\n");
				break;
			case vr::ETrackingResult::TrackingResult_Calibrating_InProgress:
				//sprintf_s(puf, sizeof(puf), "Calibrating in progress\n");
				break;
			case vr::ETrackingResult::TrackingResult_Calibrating_OutOfRange:
				//sprintf_s(puf, sizeof(puf), "Calibrating Out of range\n");
				break;
			case vr::ETrackingResult::TrackingResult_Running_OK:
				//sprintf_s(puf, sizeof(puf), "Running OK\n");
				break;
			case vr::ETrackingResult::TrackingResult_Running_OutOfRange:
				//sprintf_s(puf, sizeof(puf), "WARNING: Running Out of Range\n");
				break;
			default:
				//sprintf_s(puf, sizeof(puf), "Default\n");
				break;
			}
		}
	}
}
