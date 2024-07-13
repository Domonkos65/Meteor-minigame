This is an arduino project. I made a meteor shooter game whitch works on a 8×8 led matrix and controlled by a joystick and a button.
You can also see your score in the 4 digit 7 segment led display.

This is the gameplay in v4.2:

In the game, the key is to shoot meteorites out of the sky before they hit the ground.
First time you open it the meteor sign appears. You can press the button to go into the menu.
Then the menu appears where you can select the difficulty level by pressing the button.
If you press the button and move the joystick up, you will return to the home screen.

The game will start with 1 meteor regardless of the difficulty level and will keep increasing.
The maximum number of meteorites that appear on the screen is determined by the difficulty level (difficulty level = max meteorites).
At the MAX level, as long as you don't die, new meteors will be created.

In the game you can shoot by pressing the button. It will blow up the is closest meteor to you in the column in front of you.
You also have super powers for the characters:

      1. Supershoot
          - It can shoot more meteors in the same column 
          - joystick up + button press
          
      2. Groundshoot
          -  It can shoot more meteors in the rows next to you
          - joystick down + button press
          
You can't use these continuously like the normal shoot (it has a time limit) (the normal shoot also has a small one so you don't have to keep pressing it).
In the 4 digit 7 segment led display you can see how many meteors did you blow up.

In case one of the meteorites falls down, the game over animation and sound will be played and then you can press the button to go back to the menu.
Then the whole thing starts again.
