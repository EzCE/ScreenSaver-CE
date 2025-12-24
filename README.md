# ScreenSaver-CE

Build with `make` and send **app/bin/SCRNSAVR.8xp** and
**app/bin/AppInstA.8xv** to your calculator.

## Adding an animation

Adding an animation requires modifying a few files. The animation code should
go in its own file in **animations/src/**. After adding the code, you'll also
need to update `ANIMATION_COUNT` in **animations/src/screensavers.h**, along
with adding a function declaration for your new animaiton's launch function.

The animation's launch function should return a `bool` (set to `true` if the
hook should turn the calculator off on exit). Include **utility.h** and add
this in the animation's main loop to check if the calculator should turn off
to save battery:

```C
if (utility_ChkAPDTimer()) {
    gfx_End(); // Or whatever other clean up you need to do
    return true;
}
```

Outside of this case, the default return value should be `false`.

Next, add the launch function to the `animations` array in
**animations/src/main.c**. Finally, add a string with the name of your new
animation to the `names` array in **app/src/main.c**, and a string with the
author credited for the animation to the `authors` array in
**app/src/main.c**.

To test a screensaver animation, you can use **animations/bin/ANIMATE.8xp**,
which will run the currently selected screensaver animation.
