# How to run sleepy hollow apps without an X server

Sleepy Hollow is a library that must run on servers without a desktop
environment, but it's built on QtWebKit that needs it to render fonts
and some other stuff. This page is a compilation of strategies to
achieve that.

## xvfb

This is the easiest fix for this problem for now. You just need to run
the app using the `xvfb-run` program like in the follow example and
you're good:

```bash
 $ xvfb-run --server-args="-screen 0 1024x768x24" ./mysleepyhollowapp
```

By the way, all of our helper scripts, like `sleepy` and `make test`
will use `xvfb` if the `DISPLAY` variable is not set.


### Recompile Qt with the -qpa flag

There's a flag on Qt that allows you to choose which graphic backend Qt
should use. If you don't inform any of the available ones, it will
compile Qt without any gui stuff, which is great for us.

So, if you want to run your app without the `xvfb` stuff, you can use
this option. And if you intend to run your sleepy hollow based app on a
Debian GNU/Linux like system, you just need to follow these instructions
to make it work:

### Download the source package

```bash
$ apt-get source libqt4-dev
```

## Change the rules file

You need to add the `-qpa` flag to the `./configure` call in the
`override_dh_auto_configure` section.

Notice that you don't need to remove the `-no-webkit` flag, cause the
qtwebkit library is installed by a separate package.

ps.: I didn't test this strategy yet, so it will probably fail! :)

# Wait for Qt5

There's a patch that separates the `QtWebKitWidgets` from the `QtWebKit`
binding and ships two different shared libraries. So we'll probably give
it a try when this version becomes stable.
