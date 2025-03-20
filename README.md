# ScreenSaver-CE

Build with `make` and send **program/bin/SCRNSAVR.8xp** and
**appvar/bin/ScrnSavr.8vp** to your calculator.

## Adding an animation

Adding an animation requires modifying a few files. The animation code should
go in its own file in **appvar/src/**. After adding the code, you'll also need
to update `ANIMATION_COUNT` in **appvar/src/screensavers.h**, along with adding
a function declaration for your new animaiton's launch function.

Next, add the launch function to the `animations` array in
**appvar/src/main.c**. Finally, add a string with the name of your new
animation to the `names` array in **program/src/main.c**.
