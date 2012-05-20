/*
 YroJackGuitar - a software synthesizer based on excelent work
 of Rakkarack team

 Copyright (C) 2002-2005 Yannick Roffin
 Author: Yannick Roffin

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License (version 2)
 along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>

#include <core/YroParamHelper.h>
#include <jack/YroEffectFactory.h>
#include <jack/YroJackDriver.h>
#include <plugins/YroEffectGenerator.h>

using namespace std;

YroJackDriver *YroJackDriver::__instance = 0;

/**
 * constructor
 */
YroJackDriver::YroJackDriver() {
	LOG->info("Create instance for YroJackDriver ...");
	/**
	 * jack session handler
	 */
	client = 0;
}

/**
 * release resources
 * delete this jack driver
 */
YroJackDriver::~YroJackDriver() {
	if(client != 0) {
		LOG->info((const char *) "release jack session", 0);
		jack_deactivate(client);
		usleep(500000);
		jack_client_close(client);
		usleep(500000);
	}
	delete effectFactory;
	delete audioSampleFactory;
}

/**
 * callback
 * async method called by jack driver thread
 */
int _process(jack_nframes_t nframes, void *arg) {
	try {
		return (((YroJackDriver *) arg)->process(nframes));
	} catch(YroInternalException &e) {
		((YroJackDriver *) arg)->getLogger()->error(e.what());
	}
	return (-1);
}

/**
 * callback
 * async method called by jack driver thread
 */
void _jackShutdown(void *arg) {
	return (((YroJackDriver *) arg)->jackShutdown());
}

/**
 * process stream
 * jack_nframes_t nframes is the size of this new chunk of data to produce
 */
int YroJackDriver::process(jack_nframes_t nframes) {
	jack_default_audio_sample_t *in1, *in2, *out1, *out2;

	in1 = (jack_default_audio_sample_t*) jack_port_get_buffer(input_port1, nframes);
	in2 = (jack_default_audio_sample_t*) jack_port_get_buffer(input_port2, nframes);
	out1 = (jack_default_audio_sample_t*) jack_port_get_buffer(output_port1, nframes);
	out2 = (jack_default_audio_sample_t*) jack_port_get_buffer(output_port2, nframes);

	/**
	 * process event
	 * array size is based on Period size defined
	 * in global YroParam
	 */
	int result = effectFactory->render(in1,in2,out1,out2);

	return (result);
}

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 * async method called by jack driver thread
 */
void YroJackDriver::jackShutdown() {
	LOG->info((const char *) "JACK server stopped", 0);
}

/**
 * initialize jack driver
 * @return -1 unable to connect to jack system
 */
int YroJackDriver::initialize() {
	const char **ports;
	const char *client_name = strdup("YroJackGuitar");
	const char *server_name = NULL;

	jack_options_t options = JackNullOption;
	jack_status_t status;

    /* open a client connection to the JACK server */

	client = jack_client_open(client_name, options, &status, server_name);
	if (client == NULL) {
		LOG->error("jack_client_open() failed, status = 0x%2.0x", status);
		if (status & JackServerFailed) {
			LOG->error("Unable to connect to JACK server");
		}
		return (-1);
	}
	if (status & JackServerStarted) {
		LOG->info("JACK server started");
	}
	if (status & JackNameNotUnique) {
		client_name = jack_get_client_name(client);
		LOG->info("unique name `%s' assigned", client_name);
	}
	LOG->info("Jack server init ok");

	/* tell the JACK server to call `process()' whenever
	 there is work to be done.
	 */
	jack_set_process_callback(client, _process, (void *) this);
	LOG->info("callback onProcess available");

	/* tell the JACK server to call `jack_shutdown()' if
	 it ever shuts down, either entirely, or if it
	 just decides to stop calling us.
	 */

	jack_on_shutdown(client, _jackShutdown, (void *) this);
	LOG->info("callback onShutdown available");

	/*
	 * create two ports for input
	 * handling
	 **/
	input_port1 = jack_port_register(client, "in_1",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

	input_port2 = jack_port_register(client, "in_2",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

	if ((input_port1 == NULL) || (input_port2 == NULL)) {
		LOG->error("no more JACK ports available for input");
		return (-2);
	}
	LOG->info("input JACK ports available now");

	/*
	 * create two ports for output
	 * handling
	 **/
	output_port1 = jack_port_register(client, "out_1",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	output_port2 = jack_port_register(client, "out_2",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	if ((output_port1 == NULL) || (output_port2 == NULL)) {
		LOG->error("no more JACK ports available");
		return (-3);
	}
	LOG->info("output JACK ports available now");

	/**
	 * fixe sample rate and frame size
	 */
	YroParamHelper::instance()->setIntegerSampleRate(jack_get_sample_rate (client));
	YroParamHelper::instance()->setIntegerPeriod(jack_get_buffer_size (client));

	/**
	 * processing helper
	 * - YroEffectFactory the effects factory (loaded with default configuration)
	 * - YroAudioSampleFactory the sample factory (shared by all effects)
	 */
	effectFactory = YroEffectFactory::instance();
	effectFactory->load(0);
	audioSampleFactory = YroAudioSampleFactory::instance();

	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */

	if (jack_activate(client)) {
		LOG->error("cannot activate client");
		return (-4);
	}

	/*
	 * Connect the ports.  You can't do this before the client is
	 * activated, because we can't make connections to clients
	 * that aren't running.  Note the confusing (but necessary)
	 * orientation of the driver backend ports: playback ports are
	 * "input" to the backend, and capture ports are "output" from
	 * it.
	 */

	ports = jack_get_ports(client, NULL, NULL,
			JackPortIsPhysical | JackPortIsInput);
	if (ports == NULL) {
		LOG->error("no physical playback ports");
		return (-5);
	}

	if (jack_connect(client, jack_port_name(output_port1), ports[0])) {
		LOG->error("cannot connect output ports");
	}
	LOG->info("connect output port %s to %s", jack_port_name(output_port1), ports[0]);

	if (jack_connect(client, jack_port_name(output_port2), ports[1])) {
		LOG->error("cannot connect output ports");
	}
	LOG->info("connect output port %s to %s", jack_port_name(output_port2), ports[1]);

	free(ports);

	ports = jack_get_ports(client, NULL, NULL,
			JackPortIsPhysical | JackPortIsOutput);
	if (ports == NULL) {
		LOG->error("no physical capture ports");
		return (-6);
	}

	if (jack_connect(client, ports[0], jack_port_name(input_port1))) {
		LOG->error("cannot connect input ports");
	}
	LOG->info("connect output port %s to %s", jack_port_name(input_port1), ports[0]);

	if (jack_connect(client, ports[1], jack_port_name(input_port2))) {
		LOG->error("cannot connect input ports");
	}
	LOG->info("connect input port %s to %s", jack_port_name(input_port2), ports[1]);

	free(ports);
	return (0);
}

YroEffectFactory* YroJackDriver::getEffectFactory() const {
	return effectFactory;
}


