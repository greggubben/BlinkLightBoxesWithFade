Have you ever wanted to scare Halloween Trick-or-Treaters with something scary in your basement window?

Well, this Arduino code will turn on/off a set of LEDs used to light up whatever scary item you can find. It expects 1 box or window (set of LEDs) per pin and must use a PWM pin. **NOTE**: Make sure you do not overload your Arduino with too many LEDs attached to a single pin!

Created by: Gregg Ubben<br>
Created on: 22-Oct-2013<br>

The LED Light fade is fully configurable with ranges.  A random number is generated between the *min* and *max* values specified.  If you do not want random intervales then set the *min* and *max* to the same value. If you do not want to use the fade effects set the *min* and *max* values for the *fade_in* and *fade_out* to 0.

The following describes the options and process, assuming we are staring in an off state...

The Lights will fade in at a random interval between *fade_in_min* and *fade_in_max*.

Then the Lights will stay on for a random interval between *on_min* and *on_max*.

At which point the Lights will start to fade out for a random intervale between *fade_out_min* and *fade_out_max*.

The Lights will stay off for a random interval between *off_min* and *off_max*.

Each set of Lights also has an intial state which indicates where the Lights start in the sequence described above.  If the initial state is *BOX_OFF* then the Lights will stay off for the *off* random interval. If the initial state is *BOX_ON* then the Lights will stay on for the *on* random interval.

