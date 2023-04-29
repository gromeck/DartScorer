# Which words are used for an audio caller?

DartScorer will search for the following files in


```
  File Name		spoken content
  --------------------------------------------
  out.wav       Out!
  bull.wav      Bull!
  bullseye.wav  Bullseye!
  double.wav    Double!
  tripple.wav   Tripple!
  no-score.wav  No score!
  game-on.wav   Game On!
  won-leg.wav   You won the leg!
  won-set.wav   You won the set!
  won-game.wav  You won the game!
  busted.wav    You are busted!
  0.wav         0!
  1.wav         1!
  :             :
  180.wav       180!
```

The exclamation mark ensures a better result in text to speech.


# How to generate a voice?

* Your voice

- Get a microphone & audacity and speak the words
- cut the sounds into pieces and save the result in the named files.

* espeak.de
* espeak.en

- Install espeak
- Run the script generate-espeak.sh
- This will generate the german and english caller voice.


# Need more voices?

Visit https://github.com/gromeck/DartScorer-Voices
