# Sleepy Hollow

![http://f.cl.ly/items/0O350E410p1Y2w18292F/sleepy-hollow.jpg](http://f.cl.ly/items/0O350E410p1Y2w18292F/sleepy-hollow.jpg)

This is our exclusive badass headless browser aimed on what we do
best: scraping.


## Rationale

We need to scrape websites after processing CSS and JS, therefore we
need a full-stack browser and yet, we need it to be headless.

## Hacking

### install dependencies


#### 1. Install [QT](http://qt-project.org/doc/qt-4.8/qtwebkit.html), the core library for the browser

```bash
brew upgrade
brew install qt
```

#### 2. Create a virtual env

```bash
mkvirtualenv sleepy-hollow
```

#### 3. Use the `sleepy` command to test it

```bash
./sleepy test
```

### distributing

```bash
./sleepy release
```


![http://f.cl.ly/items/1j030T0w1J2V1O171F0N/Screen%20Shot%202012-12-03%20at%201.22.52%20PM.png](http://f.cl.ly/items/1j030T0w1J2V1O171F0N/Screen%20Shot%202012-12-03%20at%201.22.52%20PM.png)
