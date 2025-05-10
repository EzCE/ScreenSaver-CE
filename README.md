# ScreenSaver-CE

Build with `make` and send **app/bin/SCRNSAVR.8xp** and
**app/bin/AppInstA.8xv** to your calculator.

## Adding an animation

Adding an animation requires modifying a few files. The animation code should
go in its own file in **animations/src/**. After adding the code, you'll also
need to update `ANIMATION_COUNT` in **animations/src/screensavers.h**, along
with adding a function declaration for your new animaiton's launch function.

Next, add the launch function to the `animations` array in
**animations/src/main.c**. Finally, add a string with the name of your new
animation to the `names` array in **app/src/main.c**, and a string with the
author credited for the animation to the `authors` array in
**app/src/main.c**.

To test a screensaver animation, you can use **animations/bin/ANIMATE.8xp**,
which will run the currently selected screensaver animation.
