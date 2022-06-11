Audio
=====

Handling and playback of audio-files. Supported formats include: .wav, .ogg, .mp3, .flac

-----------------------

Sound
*****

ts::Sound
^^^^^^^^^

In telescope, a :code:`ts::Sound` represents any piece of audio data, usually of short length. Unlike with :code:`ts::Music`, more than
one sound can be played at the same time. To manage sound playback and memory data, telescope provides :code:`ts::SoundHandler`.
Before that, let's look at :code:`ts::Sound` itself:

.. doxygenclass:: ts::Sound
    :members:

-----------------------

Loading a Sound
^^^^^^^^^^^^^^^

If we have a small sound file :code:`ok_desu_ka.mp3` in the folder :code:`/usr/share/telescope/test/`, then we can load
the sound into memory like so:

.. code-block:: cpp
    auto sound = ts::Sound();
    sound.load("/usr/share/telescope/test/ok_desu_ka.mp3")

    // user sound here

    sound.unload(); // frees memory

The :code:`ts::Sound` automatically calls :code:`unload` upon destruction. This means, if you store all your
sounds in a simple :code:`std::vector<ts::Sound>`, the sound memory will be freed once that element is erased from the
vector.

-----------------------

ts::SoundHandler
^^^^^^^^^^^^^^^^

As mentioned before, playback of sounds is governed by :code:`ts::SoundHandler`. This class is a C++ singleton, meaning
only one SoundHandler exists over the course of runtime. By default, it supports 256 sound channels. These channels
are initialized on :code:`ts::initialize`, and can be use from that point on.

Each channel has an id from 0 to 255. In telescope, we don't manipulate sounds directly, instead we play them on a specific
channel and we can then manipulate that channel.

:code:`ts::SoundHandler` provides the following functions:

.. doxygenclass:: ts::SoundHandler
    :members:

-----------------------

Playing a Sound
^^^^^^^^^^^^^^^

Extending our previous example of loading a sound, we can then play it like so:

.. code-block:: cpp

    // load sound
    auto sound = ts::Sound("/usr/share/telescope/test/ok_desu_ka.mp3");

    // play the sound on any currently free channel
    auto channel = ts::SoundHandler::next_free_channel();
    ts::SoundHandler::play(channel, sound)

We can use the additional parameters of :code:`ts::SoundHandler::play` to control fade-in, number of
repeats. :code:`ts::SoundHandler::stop` allows us to smoothly fade-out a sound.

:code:`ts::SoundHandler` offers basic effects such as volume control and panning through :code:`set/get_volume` and
:code:`set/get_panning` respectively.

It is often impossible to load all sounds used by a game at once and keep them all in RAM, so users are encouraged
to use the tools provided by telescope to write their own sound library system.

--------------------------

--------------------------

Music
*****

ts::Music
^^^^^^^^^

Even though both classes can load the same sound file, the difference between :code:`ts::Sound` and :code:`ts::Music`
is that only one music can play at the same time. There are no channels, there is only the currently playing music.

.. doxygenclass:: ts::Music
    :members:

-----------------------------

Loading Music
^^^^^^^^^^^^^

Music is loaded just like sounds are:

.. code-block:: cpp

    auto music = ts::Music();
    music.load("/usr/share/telescope/test/ok_desu_ka.mp3")

    // use music here

    music.unload();

Again, it may be more convenient to handle loading / unloading through clever use of :code:`ts::Music`s destructor.

-----------------------------

ts::MusicHandler
^^^^^^^^^^^^^^^^

Telescope provides its own handler for music: :code:`ts::MusicHandler`. This class is, similarly, a singleton. Unlike
:code:`ts::MusicHandler`, however, :code:`ts::MusicHandler` gives us much more control over the audio file currently
being played:

.. doxygenclass:: ts::MusicHandler
    :members:

-----------------------------

Playing Music
^^^^^^^^^^^^^

We see that we can skip around the current playback position of the music like so:

.. code-block:: cpp

    // load 3s long audio file as music
    auto music = ts::Music("/usr/share/telescope/test/ok_desu_ka.mp3");
    ts::MusicHandler::play(music);

    // wait 1s
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // skip to the beginning of the audio file
    ts::MusicHandler::skip_to(ts::seconds(0));

This allows users to loop a certain part of the music, repeat the track from the beginning or skip to the end. The
latter is useful because telescope allows us to queue up to one track using :code:`ts::MusicHandler::play_next`.

.. code-block:: cpp

    auto music_first = ts::Music(" // ...
    auto music_second = ts::Musci(" // ...

    // start music_first playback
    ts::MusicHandler::play(music_first);

    // queue music_second to start when the current playback ends
    ts::MusicHandler::play_next(music_second);

    // if we don't want to wait for the runtime to end,
    // we can trigger the next song immediately like so:
    ts::MusicHandler::next();

Other than :code:`ts::MusicHandler::next`, the queued music (if any) is also played when calling
:code:`MusicHandler::stop`. because :code:`play_next` allows us to specify a fade-in ramp duration and :code:`stop`
or :code:`next` allows us to specify a fade-out ramp duration, we can smoothly cross-fade from one song to the other like so:

.. code-block:: cpp

    auto music_first = ts::Music(" // ...
    auto music_second = ts::Musci(" // ...

    // start music_first playback immediately
    ts::MusicHandler::play(music_first);

    auto fade_in_duration = ts::seconds(0.3);
    auto fade_out_duration = ts::seconds(1.5)

    // register fade-in duration
    ts::MusicHandler::play_next(music_second, fade_in_duration);

    // wait for playback here

    // then, when we are ready, register fade-out duration
    ts::MusicHandler::next(fade_out_duration);

-----------------------------

In summary, using :code:`ts::MusicHandler` and :code:`ts::SoundHandler`, we are given the tools
for handling audio in all but the most demanding situations. To address this, sound-effects and signal-processing of audio data
are planned for a future version of telescope.





