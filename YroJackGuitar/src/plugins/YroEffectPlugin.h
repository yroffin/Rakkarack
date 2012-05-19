/*
 * YroEffectPlugin.h
 *
 *  Created on: 5 mai 2012
 *      Author: yannick
 */

#include <wx/WxApp.h>
#include <wx/spinctrl.h>

#include <core/YroObject.h>
#include <jack/jack.h>
#include <jack/YroAudioSampleFactory.h>
#include <plugins/YroPreset.h>
#include <plugins/YroPresets.h>

#ifndef YROEFFECTPLUGIN_H_
#define YROEFFECTPLUGIN_H_

#define D_PI 6.283185f
#define PI 3.141598f
#define LOG_10 2.302585f
#define LOG_2  0.693147f
#define LN2R 1.442695041f
#define CNST_E  2.71828182845905f
#define AMPLITUDE_INTERPOLATION_THRESHOLD 0.0001f
#define FF_MAX_VOWELS 6
#define FF_MAX_FORMANTS 12
#define FF_MAX_SEQUENCE 8
#define MAX_FILTER_STAGES 5
#define RND (rand()/(RAND_MAX+1.0))
#define RND1 (((float) rand())/(((float) RAND_MAX)+1.0f))
#define F2I(f,i) (i)=((f>0) ? ( (int)(f) ) :( (int)(f-1.0f) ))
#define dB2rap(dB) (float)((expf((dB)*LOG_10/20.0f)))
#define rap2dB(rap) (float)((20*log(rap)/LOG_10))
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define INTERPOLATE_AMPLITUDE(a,b,x,size) ( (a) + ( (b) - (a) ) * (float)(x) / (float) (size) )
#define ABOVE_AMPLITUDE_THRESHOLD(a,b) ( ( 2.0f*fabs( (b) - (a) ) / ( fabs( (b) + (a) + 0.0000000001f) ) ) > AMPLITUDE_INTERPOLATION_THRESHOLD )
#define POLY 8
#define DENORMAL_GUARD 1e-18f	// Make it smaller until CPU problem re-appears
#define SwapFourBytes(data) ( (((data) >> 24) & 0x000000ff) | (((data) >> 8) & 0x0000ff00) | (((data) << 8) & 0x00ff0000) | (((data) << 24) & 0xff000000) )
#define D_NOTE          1.059463f
#define LOG_D_NOTE      0.057762f
#define D_NOTE_SQRT     1.029302f
#define MAX_PEAKS 8
#define MAX_ALIENWAH_DELAY 100
#define ATTACK  0.175f  //crossover time for reverse delay
#define MAX_DELAY 2	// Number of Seconds
#define MAXHARMS  8    // max number of harmonics available
#define MAX_PHASER_STAGES 12
#define MAX_CHORUS_DELAY 250.0f	//ms
#define LN2                       (1.0f)  //Uncomment for att/rel to behave more like a capacitor.
#define MUG_CORR_FACT  0.4f
//Crunch waveshaping constants
#define Thi		0.67f			//High threshold for limiting onset
#define Tlo		-0.65f			//Low threshold for limiting onset
#define Tlc		-0.6139445f		//Tlo + sqrt(Tlo/500)
#define Thc		0.6365834f	        //Thi - sqrt(Thi/600)
#define CRUNCH_GAIN	100.0f			//Typical voltage gain for most OD stompboxes
#define DIV_TLC_CONST   0.002f			// 1/300
#define DIV_THC_CONST	0.0016666f		// 1/600 (approximately)
//End waveshaping constants
#define D_FLANGE_MAX_DELAY	0.055f			// Number of Seconds  - 50ms corresponds to fdepth = 20 (Hz). Added some extra for padding
#define LFO_CONSTANT		9.765625e-04		// 1/(2^LOG_FMAX - 1)
#define LOG_FMAX		10.0f			//  -- This optimizes LFO sweep for useful range.
#define MINDEPTH		20.0f			// won't allow filter lower than 20Hz
#define MAXDEPTH		15000.0f		// Keeps delay greater than 2 samples at 44kHz SR
#define MAX_EQ_BANDS 16
#define CLOSED  1
#define OPENING 2
#define OPEN    3
#define CLOSING 4
#define ENV_TR 0.0001f
#define HARMONICS 11
#define REV_COMBS 8
#define REV_APS 4
#define MAX_SFILTER_STAGES 12

namespace std {

class YroEffectPlugin: public std::YroObject {
public:
	/**
	 * TODO add instance adjustment to setup as many effect of the same type as we want
	 * TODO add new parameter
	 */
	YroEffectPlugin(const char *_name);
	YroEffectPlugin(const char *_name, const char *_preset);
	virtual ~YroEffectPlugin();
	virtual void cleanup () {
	}
	void toggle();
	/**
	 * processing
	 */
	virtual void render(jack_nframes_t nframes, float *inLeft, float *inRight) {
	}
	virtual void render(float *inLeft, float *inRight) {
		render(iPERIOD,inLeft,inRight);
	}
	virtual void setPreset(int npreset);
	int getPreset() const {
		return preset;
	}

	/**
	 * getter
	 */
	float *getOutLeft();
	float *getOutRight();
	void setOutLeft(float *value);
	void setOutRight(float *value);
	/**
	 * inline
	 */
	const char *getName() {
		return name;
	}
	/**
	 * Wx widget linker
	 * - link link this field to this widget
	 * - onChange advertise change on widget
	 */
	void subscribe(int index, wxSpinCtrl *widget);
	void onChange(wxSpinCtrl *widget);
	void subscribe(int index, wxChoice *widget);
	void onChange(wxChoice *widget);
	void subscribe(int index, wxSlider *widget);
	void onChange(wxSlider *widget);
	void subscribe(int index, wxCheckBox *widget);
	void onChange(wxCheckBox *widget);
	void onChange(int index);

	int getInt(int index);
	void setInt(int index, int value);
	int getActive() const;
	void setActive(int active);

	virtual int  get0() {return 0;}; virtual void set0(int) {};
	virtual int  get1() {return 0;}; virtual void set1(int) {};
	virtual int  get2() {return 0;}; virtual void set2(int) {};
	virtual int  get3() {return 0;}; virtual void set3(int) {};
	virtual int  get4() {return 0;}; virtual void set4(int) {};
	virtual int  get5() {return 0;}; virtual void set5(int) {};
	virtual int  get6() {return 0;}; virtual void set6(int) {};
	virtual int  get7() {return 0;}; virtual void set7(int) {};
	virtual int  get8() {return 0;}; virtual void set8(int) {};
	virtual int  get9() {return 0;}; virtual void set9(int) {};
	virtual int  get10() {return 0;}; virtual void set10(int) {};
	virtual int  get11() {return 0;}; virtual void set11(int) {};
	virtual int  get12() {return 0;}; virtual void set12(int) {};
	virtual int  get13() {return 0;}; virtual void set13(int) {};
	virtual int  get14() {return 0;}; virtual void set14(int) {};
	virtual int  get15() {return 0;}; virtual void set15(int) {};
	virtual int  get16() {return 0;}; virtual void set16(int) {};
	virtual int  get17() {return 0;}; virtual void set17(int) {};
	virtual int  get18() {return 0;}; virtual void set18(int) {};
	virtual int  get19() {return 0;}; virtual void set19(int) {};
protected:
	map<wxSpinCtrl *, int> mapSpinCtrlIndex;
	map<int, wxSpinCtrl *>  mapIndexSpinCtrl;
	map<wxChoice *, int> mapChoiceIndex;
	map<int, wxChoice *>  mapIndexChoice;
	map<wxSlider *, int> mapSliderIndex;
	map<int, wxSlider *>  mapIndexSlider;
	map<wxCheckBox *, int> mapCheckBoxIndex;
	map<int, wxCheckBox *>  mapIndexCheckBox;

	int active;

	int preset;
	float *efxoutl;
	float *efxoutr;
	const char *name;

	int   iPERIOD;
	int   iSAMPLE_RATE;
	float fPERIOD;
	float fSAMPLE_RATE;
	float cSAMPLE_RATE;
	YroAudioSampleFactory *audioSampleFactory;
	YroPresets *presets;
};

} /* namespace std */

#endif /* YROEFFECTPLUGIN_H_ */
