# ws_opengl_works
OpenGLで作った作品一覧

## 遊び方
特別必要なライブラリの指定がなければそのままOpenGLに必要なライブラリを持ってきてコンパイルして遊ぶだけ。

## コンパイル
### Linux
```sh
g++ main.cpp -lglut -lGLU -lGL
```
### Mac
`-DGL_SILENCE_DEPRECATION` はうるさい警告を消す魔法
```sh
g++ main.cpp -framework OpenGL -framework GLUT -DGL_SILENCE_DEPRECATION
```
### Windows
いろいろな方法がある(?)ので検索してください。

## 存在しないヘッダファイル
以下のファイルは自作のものではないためここでアップロードしていません。自分で探して追加してください。

ただ、入れただけでは動きません。いくつか加える関数などが必要です。多分いずれそれについて書く。

* vec_op.hpp
* jfont.h
