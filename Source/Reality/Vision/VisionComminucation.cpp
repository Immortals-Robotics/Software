#include "Vision.h"

bool VisionModule::connectToVisionServer ( const std::string & address , const unsigned short port )
{
	try
	{
		VisionUDP = new UDPSocket ( port );

		VisionUDP -> joinGroup ( address );

		connected = true;

		return true;

	}

	catch ( SocketException ex )
	{
		connected = false;

		return false;
	}
}

bool VisionModule::recievePacket ( void )
{
	if ( ! connected )
		return false;

	try{
		int incoming_size = VisionUDP -> recv ( incoming_buffer , MAX_INCOMING_PACKET_SIZE );

		packet.ParseFromArray(incoming_buffer,incoming_size);
	}
	catch(...)
	{
		return false;
	}

	if ( packet.has_detection ( ) )
	{
		frame[packet.detection ( ).camera_id ( )] = packet.detection ( );
		packet_recieved[packet.detection ( ).camera_id ( )] = true;
	}
	else
		return false;

	return true;

}

bool VisionModule::isConnected ( void )
{
	return connected;
}
void VisionModule::SendGUIData ( WorldState * state , AI_Debug & aidebug )
{
	if ( state == NULL )
		return;
	try
	{
		Ai2GuiMsg GUIMsg;
		
		GUIMsg.set_timestamp(0);
		
		aidebug.set_frame_id(0);
		
		GUIMsg.mutable_aidbgdata()->CopyFrom(aidebug);
		
		GUIMsg.mutable_ballsdata() -> set_seenstate ( state -> ball.seenState );
		GUIMsg.mutable_ballsdata() -> set_vdir ( state -> ball.velocity.direction );
		GUIMsg.mutable_ballsdata() -> set_vmag ( state -> ball.velocity.magnitude );
		GUIMsg.mutable_ballsdata() -> set_vx ( state -> ball.velocity.x );
		GUIMsg.mutable_ballsdata() -> set_vy ( state -> ball.velocity.y );
		GUIMsg.mutable_ballsdata() -> set_x ( state -> ball.Position.X );
		GUIMsg.mutable_ballsdata() -> set_y ( state -> ball.Position.Y );

		for ( int i = 0 ; i < 12 ; i ++ )
		{
			robotPacket[0][i].Clear ( );
			robotPacket[0][i].set_omega ( state -> OwnRobot[i].AngularVelocity );
			robotPacket[0][i].set_orientation ( state -> OwnRobot[i].Angle );
			robotPacket[0][i].set_robot_id ( state -> OwnRobot[i].vision_id );
			robotPacket[0][i].set_seenstate ( state -> OwnRobot[i].seenState );
			robotPacket[0][i].set_vdir ( state -> OwnRobot[i].velocity.direction );
			robotPacket[0][i].set_vmag ( state -> OwnRobot[i].velocity.magnitude );
			robotPacket[0][i].set_vx ( state -> OwnRobot[i].velocity.x );
			robotPacket[0][i].set_vy ( state -> OwnRobot[i].velocity.y );
			robotPacket[0][i].set_x ( state -> OwnRobot[i].Position.X );
			robotPacket[0][i].set_y ( state -> OwnRobot[i].Position.Y );
			robotPacket[0][i].set_vmag ( state -> lastCMDS[i][(int)(state->lastCMDS[i][10].X)].Y * 45.0f );
			robotPacket[0][i].set_own ( 1 );
			if (state -> OwnRobot[i].seenState!=CompletelyOut)
			{
				robotDataMsg *tmprobot = GUIMsg.add_robotdata ( );
				tmprobot -> CopyFrom ( robotPacket[0][i] );
			}
		}

		for ( int i = 0 ; i < 12 ; i ++ )
		{
			robotPacket[1][i].Clear ( );
			robotPacket[1][i].set_omega ( state -> OppRobot[i].AngularVelocity );
			robotPacket[1][i].set_orientation ( state -> OppRobot[i].Angle );
			robotPacket[1][i].set_robot_id ( state -> OppRobot[i].vision_id );
			robotPacket[1][i].set_seenstate ( state -> OppRobot[i].seenState );
			robotPacket[1][i].set_vdir ( state -> OppRobot[i].velocity.direction );
			robotPacket[1][i].set_vmag ( state -> OppRobot[i].velocity.magnitude );
			robotPacket[1][i].set_vx ( state -> OppRobot[i].velocity.x );
			robotPacket[1][i].set_vy ( state -> OppRobot[i].velocity.y );
			robotPacket[1][i].set_x ( state -> OppRobot[i].Position.X );
			robotPacket[1][i].set_y ( state -> OppRobot[i].Position.Y );
			robotPacket[1][i].set_own ( 0 );
			if (state -> OppRobot[i].seenState!=CompletelyOut)
			{
				robotDataMsg *tmprobot = GUIMsg.add_robotdata ( );
				tmprobot -> CopyFrom ( robotPacket[1][i] );
			}
		}

		GUIMsg.mutable_gamestate()->set_goalblue ( state -> refereeState.goals_blue );
		GUIMsg.mutable_gamestate()->set_goalyellow ( state -> refereeState.goals_yellow );
		GUIMsg.mutable_gamestate()->set_timeremaining ( state -> refereeState.time_remaining );
		GUIMsg.mutable_gamestate()->set_refcmd ( "a" );
		GUIMsg.mutable_gamestate()->set_state ( "a" );
		GUIMsg.mutable_gamestate()->set_probgoalblue ( false );
		GUIMsg.mutable_gamestate()->set_probgoalyellow ( false );
		if ( state -> refereeState.State )
		{
		}

		string GUIBuffer;

		GUIMsg.SerializeToString ( &GUIBuffer );

		//GUIUDP -> sendTo ( GUIBuffer.c_str() , GUIBuffer.length() , "224.5.66.6" , 10009 );
		zmq_send (gui_zmq_publisher, GUIBuffer.c_str(), GUIBuffer.size(), 0);
		

	}
	catch(...)
	{
		std::cout << "hehe" << std::endl;
	}
}
