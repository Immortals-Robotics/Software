#include "proto_bridge.h"

#include "../protos/messages_immortals_robot_wrapper.pb.h"
#include "data_lite.h"
#include "writer.h"
#include "reader.h"

size_t proto_msg_frame_to_byte_array(const Immortals::Data::RobotMessageFrame &data, uint8_t *const buffer)
{
	size_t size = 0;

	for (const auto &message : data.messages())
	{
		robot_wrapper_msg_t wrapper_msg;

		if (message.body_case() == Immortals::Data::RobotMessage::kCommand)
		{
			const Immortals::Data::RobotCommand &command = message.command();
			robot_command_msg_t command_msg;

			if (command.has_velocity())
			{
				command_msg.velocity.x.f32 = command.velocity().x();
				command_msg.velocity.y.f32 = command.velocity().y();
				command_msg.halt = false;
			}
			else
			{
				command_msg.velocity.x.f32 = 0.0f;
				command_msg.velocity.y.f32 = 0.0f;
				command_msg.halt = true;
			}

			command_msg.omega.f32 = command.omega();
			command_msg.target_orientation.f32 = command.target_orientation();

			if (command.has_orientation())
			{
				command_msg.orientation.f32 = command.orientation();
				command_msg.has_orientation = true;
			}
			else
			{
				command_msg.orientation.f32 = 0.0f;
				command_msg.has_orientation = false;
			}
			

			if (command.shoot_case() == Immortals::Data::RobotCommand::kDirect)
			{
				command_msg.shoot_power.f32 = command.direct();
				command_msg.shoot_type = SHOOT_TYPE_DIRECT;
			}
			else if (command.shoot_case() == Immortals::Data::RobotCommand::kChip)
			{
				command_msg.shoot_power.f32 = command.chip();
				command_msg.shoot_type = SHOOT_TYPE_CHIP;
			}
			else if (command.shoot_case() == Immortals::Data::RobotCommand::SHOOT_NOT_SET)
			{
				command_msg.shoot_power.f32 = 0.0f;
				command_msg.shoot_type = SHOOT_TYPE_DIRECT;
			}

			command_msg.dribbler.f32 = command.dribbler();
			command_msg.servo.f32 = command.servo();

			command_msg.beep = static_cast<uint8_t>(command.beep());

			command_msg.feedback_request = static_cast<feedback_type_e>(command.feedback());

			wrapper_msg.length = static_cast<uint8_t>(write_robot_command_fixed(wrapper_msg.data, &command_msg));
		}

		else if (message.body_case() == Immortals::Data::RobotMessage::kControlConfig)
		{
			const Immortals::Data::RobotControlConfig &config = message.control_config();
			robot_control_config_msg_t config_msg;

			config_msg.motor_kp.f32 = config.motor_kp();
			config_msg.motor_ki.f32 = config.motor_ki();
			config_msg.motor_kd.f32 = config.motor_kd();
			config_msg.motor_i_limit.f32 = config.motor_i_limit();

			config_msg.gyro_kp.f32 = config.gyro_kp();
			config_msg.gyro_ki.f32 = config.gyro_ki();
			config_msg.gyro_kd.f32 = config.gyro_kd();
			config_msg.gyro_i_limit.f32 = config.gyro_i_limit();

			config_msg.max_w_acc.f32 = config.max_w_acc();
			config_msg.max_w_dec.f32 = config.max_w_dec();

			wrapper_msg.length = static_cast<uint8_t>(write_robot_control_config_fixed(wrapper_msg.data, &config_msg));
		}

		else if (message.body_case() == Immortals::Data::RobotMessage::kShootConfig)
		{
			const Immortals::Data::RobotShootConfig &config = message.shoot_config();
			robot_shoot_config_msg_t config_msg;

			config_msg.direct_coeffs.x.f32 = config.direct_coeffs().x();
			config_msg.direct_coeffs.y.f32 = config.direct_coeffs().y();
			config_msg.direct_coeffs.z.f32 = config.direct_coeffs().z();

			config_msg.chip_coeffs.x.f32 = config.chip_coeffs().x();
			config_msg.chip_coeffs.y.f32 = config.chip_coeffs().y();
			config_msg.chip_coeffs.z.f32 = config.chip_coeffs().z();

			wrapper_msg.length = static_cast<uint8_t>(write_robot_shoot_config_fixed(wrapper_msg.data, &config_msg));
		}
		else if (message.body_case() == Immortals::Data::RobotMessage::kMatrix)
		{
			// TODO
			continue;
		}
		else
		{
			// TODO: print error msg
			continue;
		}

		buffer[size] = static_cast<uint8_t>(message.robot_id());
		size++;
		size += write_robot_wrapper_fixed(buffer + size, &wrapper_msg);
	}

	return size;
}

bool feedback_bytes_to_proto_feedback(const uint8_t *const buffer, const size_t size, Immortals::Data::RobotMessage *const proto_msg)
{
	uint8_t result;

	robot_wrapper_msg_t wrapper_msg;
	result = read_robot_wrapper_fixed(buffer, size, &wrapper_msg);
	if (result != PARSE_RESULT_SUCCESS)
		return false;

	robot_feedback_msg_t message;
	result = read_robot_feedback_fixed(wrapper_msg.data, wrapper_msg.length, &message);
	if (result != PARSE_RESULT_SUCCESS)
		return false;

	proto_msg->set_robot_id(message.robot_id);
	
	auto feedback = proto_msg->mutable_feedback();

	feedback->Clear();

	feedback->set_battery_voltage(message.battery_voltage.f32);
	feedback->set_capacitor_voltage(message.capacitor_voltage.f32);
	
	feedback->set_omega(message.omega.f32);
	feedback->set_orientation(message.orientation.f32);

	feedback->mutable_motor_velocity()->set_x(message.motor_velocity.x.f32);
	feedback->mutable_motor_velocity()->set_y(message.motor_velocity.y.f32);
	feedback->mutable_motor_velocity()->set_z(message.motor_velocity.z.f32);
	feedback->mutable_motor_velocity()->set_w(message.motor_velocity.w.f32);

	feedback->mutable_motor_target()->set_x(message.motor_target.x.f32);
	feedback->mutable_motor_target()->set_y(message.motor_target.y.f32);
	feedback->mutable_motor_target()->set_z(message.motor_target.z.f32);
	feedback->mutable_motor_target()->set_w(message.motor_target.w.f32);

	feedback->add_motor_fault(message.motor_fault.bit0);
	feedback->add_motor_fault(message.motor_fault.bit1);
	feedback->add_motor_fault(message.motor_fault.bit2);
	feedback->add_motor_fault(message.motor_fault.bit3);

	feedback->add_encoder_fault(message.motor_fault.bit4);
	feedback->add_encoder_fault(message.motor_fault.bit5);
	feedback->add_encoder_fault(message.motor_fault.bit6);
	feedback->add_encoder_fault(message.motor_fault.bit7);

	feedback->add_button_status(message.button_status.bit0);
	feedback->add_button_status(message.button_status.bit1);
	feedback->add_button_status(message.button_status.bit2);
	feedback->add_button_status(message.button_status.bit3);
	feedback->add_button_status(message.button_status.bit4);
	feedback->add_button_status(message.button_status.bit5);
	feedback->add_button_status(message.button_status.bit6);
	feedback->add_button_status(message.button_status.bit7);

	feedback->set_fault(message.fault);
	feedback->set_ball_detected(message.ball_detected);
	feedback->set_booster_enabled(message.booster_enabled);
	feedback->set_dribbler_connected(message.dribbler_connected);

	return true;
}
