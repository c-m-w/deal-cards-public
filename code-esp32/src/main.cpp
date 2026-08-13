#include "main.hpp"

void update_display(void *);
void exit();
void do_pin_testing();

void setup() {
	Serial.begin(115200);
	delay(100);
	
	Serial.printf("C++ Standard: %ld\n", __cplusplus);

	xTaskCreatePinnedToCore(
		update_display,
		"DisplayTask",
		4096,
		NULL,
		1,
		NULL,
		0
	);

	Serial.printf("Running main tasks on core %d\n", xPortGetCoreID());

	if (!ctl.init())
	{
		Serial.println("setup | Controller setup failed.");
		exit();
	}
	
	//do_pin_testing();

	Serial.println("Setup complete. Waiting for signal to begin homing.");
	
	ctl._disp.set_menu(&m::_homing_prompt);

	while (!m::_homing_prompt._continue) {
		ctl._disp.process_inputs();
		delay(1);
	}

	ctl._disp.set_menu(&m::_homing);

	if (!rob.do_homing()) {
		Serial.println("setup | Homing failed.");
		exit();
	}

	ctl._disp.set_menu(&m::_tune_pid);
}

void loop()
{
	ctl._disp.process_inputs();
	delay(1);
	return;

	static unsigned long last_update_time = millis();

	auto const speed = ctl.get_ctl_speed();

	rob.move(speed);

	if (ctl._btn_enter)
	{
		rob.fire();
	}

	if (ctl._btn_up)
	{
		rob.move_to(240.f);
		rob.fire();
		rob.move_to(180.f);
		rob.fire();
		rob.move_to(120.f);
		rob.fire();
		rob.move_to(300.f);
		rob.fire();
		rob.move_to(40.f);
		rob.fire();
	}

	if (ctl._btn_dn)
	{
		rob.move_to(180.f);
	}

	delay(1);
	return;
}

void update_display(void *)
{
	Serial.printf("Started update_display() on core %d\n", xPortGetCoreID());

	while(true)
	{
		ctl._disp.show();
		delay(15);
	}
}

void exit()
{
	while(true) delay(1);
}

void do_pin_testing()
{
	Serial.println("Pin testing mode");

	while (true)
	{
		if (rob._actrl.pos_lim_hit())
		{
			Serial.println("Positive limit hit!");
		}

		
		if (rob._actrl.neg_lim_hit())
		{
			Serial.println("Negative limit hit!");
		}

		if (ctl._btn_enter)
		{
			rob._actrl.print_angle();
			Serial.println("Enter pressed!");
		}

		if (ctl._btn_up)
		{
			rob._actrl.move_positive(0.15f);
			Serial.println("Up pressed!");
		}
		else
		{
			rob._actrl.stop();
		}

		if (rob._vcnt.poll())
		{
			Serial.println("Card detected!");
		}

		if (ctl._btn_dn)
		{
			rob._feeder.feed_forward();
			rob._launcher.set_speed(0.2f);
			Serial.println("Down pressed!");
		}
		else
		{
			rob._feeder.stop();
			rob._launcher.stop();
		}

		delay(50);
	}
}
