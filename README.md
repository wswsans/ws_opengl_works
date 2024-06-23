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
以下のファイルは自作のものではないためここでアップロードしていません。自分で探してforCopyフォルダに追加してください。

ただ、入れただけでは動きません。いくつか加える関数などが必要です。多分いずれそれについて書く。
* [vec_op.hpp](https://github.com/birdwatcherYT/vec_op)
* [jfont.h](https://fujiwaratko.sakura.ne.jp/gcc/glut/glut.html)

## useful_func.cpp
`forCopy/useful_func.cpp` には、作品の中で作った関数の中で今後も使うだろうなっていう関数をまとめています。
ご自由にお使いください。

もし、使ったコードを公開するときはこのリポジトリを紹介してください。(クレジット的な?)

(こういうの、しっかりとライセンスを出せば良いのだろうけど詳しく知らなさすぎて...)

## コンパイル時に必要になる引数(インクルードファイル編)
作品をコンパイルするときに引数に追加するファイル一覧です。
各作品に使っているファイルが記載されているので、それに合わせて追加してください。

=> `...` ←この中のを引数に追加してください。

### CoordSystem.h
=> `../forCopy/CoordSystem.cpp`

cppファイルです。hファイルではありません。hファイルだとコンパイルできません。

### jfont.h
=> `../forCopy/jfont.h`

CoordSystem.hの時とは違い、こちらはhファイルです。hファイルを引数に追加してください。

### Physics.h
=> `../glPhysics/Physics.cpp`

CoordSystem.h同様、hファイルではなくcppファイルです。

## 作品解説
### Tap to On
[ライツアウト](https://ja.wikipedia.org/wiki/%E3%83%A9%E3%82%A4%E3%83%84%E3%82%A2%E3%82%A6%E3%83%88)の、ライトを付ける方です。昔iPhoneにも[Android](https://play.google.com/store/apps/details?id=com.gameday.TapToOn_Google)にもあったのですがストアから消されたので自分で作りました。ただゲーム部分だけであって全部再現というわけではありません。

mapフォルダにあるテキストファイルは、ライトの数やONの位置が書いてます。一行目に縦(横)のライトの数、その下はONとOFFが書かれてあります。以下は例です。
```
4
1101
1101
1110
1111
```

小話

2021年まではストアにあったり追加パックを買えたのですが、ある日突然購入できなくなって[AmazonApp](https://www.amazon.co.jp/dp/B00BWI2YO4)に頼るしかなくなったのを許していません。

全ステージ再現をするには特別な機能を持ったライトの実装が必要になりますが、それはまたいつか。

### Collision pi
[質量が1:100^nで、完全弾性衝突をする時、衝突回数は円周率になる](https://youtu.be/a-EOA3j3tw4)という話があります。
これを再現しました。

もともと前にXWindowで再現したのですが、作り直しました。

### forCopy
あると楽だなーっていう関数をまとめました。useful_func.cppにある関数を私はコピペして使っています。

[🤖](https://dic.pixiv.net/a/%E3%83%AD%E3%83%9C%E3%83%83%E3%83%88%28%E5%AD%A6%E3%81%B3%E8%80%83%E3%81%88%E3%82%8B%E6%AD%B4%E5%8F%B2%29) < インクルードすれば良いのに...

ここに[jfont.h](https://fujiwaratko.sakura.ne.jp/gcc/glut/glut.html)や[vec_op.hpp](https://github.com/birdwatcherYT/vec_op)を入れてください。
なおプログラムの方にあるインクルード先をいじればどこでもかまいません。

### glArrow
矢印ってあったら楽だなーっていう感じで作りました。

使用ライブラリ: CoordSystem.h

### glPhysics
もとはCoordSystemです。
これに加えていろいろな概念(力、熱、電磁気等)を追加したいです。(願望)

使用ライブラリ: CoordSystem.h

#### Physics.h, Physics.cpp以外のファイル
サンプルです。すべてのコンパイルで以下の引数を追加する必要があります
`../forCopy/CoordSystem.cpp Physics.cpp`

force.cppだけforce.cppなのは、力の概念を加えたときに作ったサンプルだからです。
* projectile_motion.cpp: 放物線運動
* circle_motion.cpp: 円運動
* force.cpp: 力の概念を利用した単振動

### loading
ローディング画面(単体)です。
好きな文字を入れて好きなローディング画面を作ろう。

使用ライブラリ: jfont.h

### glCircle
円を任意の法線ベクトル(水平角, 垂直角で定められたベクトル)を使って描くプログラムです。
ほら, 任意の角度でさ, 動くのってさ, 良いじゃん?

使用ライブラリ: CoordSystem.h

### glSpiral
glCircleのコードを少しいじった結果できた渦や螺旋です。なんと、今回は一気に3種も!

使用ライブラリ: CoordSystem.h

### glCube
既存の立方体関数をいつもも感じで作り直しました。
そういえば...[glCubeに似た関数ってOpenGLライブラリに存在したような...](https://w.atwiki.jp/opengl/pages/29.html)

使用ライブラリ: CoordSystem.h
