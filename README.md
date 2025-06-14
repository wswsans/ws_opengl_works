# ws_opengl_works
OpenGLで作った作品一覧

# 遊び方
特別必要なライブラリの指定がなければそのままOpenGLに必要なライブラリを持ってきてコンパイルして遊ぶだけ。

# コンパイル
## Linux
```sh
g++ main.cpp -lglut -lGLU -lGL
```
## Mac
`-DGL_SILENCE_DEPRECATION` はうるさい警告を消す魔法
```sh
g++ main.cpp -framework OpenGL -framework GLUT -DGL_SILENCE_DEPRECATION
```
## Windows
いろいろな方法がある(?)ので検索してください。

# ヘッダファイル
## 存在しないヘッダファイル
以下のファイルは自作のものではないためここでアップロードしていません。自分で探してforCopyフォルダに追加してください。

ただ、入れただけでは動きません。いくつか加える関数などが必要です。多分いずれそれについて書く。
* [vec_op.hpp](https://github.com/birdwatcherYT/vec_op)
* [jfont.h](https://fujiwaratko.sakura.ne.jp/gcc/glut/glut.html)

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

# 作品解説
## Tap to On
[ライツアウト](https://ja.wikipedia.org/wiki/%E3%83%A9%E3%82%A4%E3%83%84%E3%82%A2%E3%82%A6%E3%83%88)の、ライトを付ける方です。昔iPhoneにも[Android](https://play.google.com/store/apps/details?id=com.gameday.TapToOn_Google)にもあったのですがストアから消されたので自分で作りました。ただゲーム部分だけであって全部再現というわけではありません。

mapフォルダにあるテキストファイルは、ライトの数やONの位置が書いてます。一行目に縦(横)のライトの数、その下はONとOFFが書かれてあります。以下は例です。
* 1行目: 行と列数
* 2行目以降: ステージの初期設定で、1がON、0がOFFで書かれている。
```
4
1101
1101
1110
1111
```

### 小話
2021年まではストアにあったり追加パックを買えたのですが、ある日突然購入できなくなって[AmazonApp](https://www.amazon.co.jp/dp/B00BWI2YO4)に頼るしかなくなったのを許していません。
全ステージ再現をするには特別な機能を持ったライトの実装が必要になりますが、それはまたいつか。

## Collision pi
[質量が1:100^nの2つの物体が、物体と壁で完全弾性衝突をする時、その衝突回数は円周率になる](https://youtu.be/a-EOA3j3tw4)という話があります。
これを再現しました。

もともと前にXWindowで再現したのですが、作り直しました。

## forCopy
あると楽だなーっていう関数をまとめました。useful_func.cppにある関数を私はコピペして使っています。

[🤖](https://dic.pixiv.net/a/%E3%83%AD%E3%83%9C%E3%83%83%E3%83%88%28%E5%AD%A6%E3%81%B3%E8%80%83%E3%81%88%E3%82%8B%E6%AD%B4%E5%8F%B2%29) < インクルードすれば良いのに...

ここに[jfont.h](https://fujiwaratko.sakura.ne.jp/gcc/glut/glut.html)や[vec_op.hpp](https://github.com/birdwatcherYT/vec_op)を入れてください。
なおプログラムの方にあるインクルード先をいじればどこでもかまいませんが、全てのコードはここにある前提で書かれています。
### useful_func.cpp
`forCopy/useful_func.cpp` には、作品の中で作った関数の中で今後も使うだろうなっていう関数をまとめています。
```cpp

/*
 * vectorの中を表示する
sample:
vector<int> sample = {1, 2, 3};
print_vec("テスト", sample)
=> テスト: (1, 2, 3)
*/
template<typename T>
void print_vec(string name, vector<T> vec)

/*
 * https://suzulang.com/opengl-render-char-only-ascii/
 * (x, y, z)を基準にstrを表示する
 * 日本語対応
*/
void render_string(float x, float y, float z, const char* str)

/*
 * require: CommonCoordSystem
 * (x0, y0, z0)を中心に1辺がlengthで、向きがオイラー角(x, y, z) = (vertical, 0, horizontal)の直方体を作成
*/
void glCube(double x0, double y0, double z0, double length, double horizontal, double vertical)

/*
 * require: CommonCoordSystem
 * (x0, y0, z0)を中心に1辺がlengthで、向きがオイラー角(x, y, z) = (vertical, 0, horizontal)の矢印を作成
 * もう一つは、pos0 = (x0, y0, z0)、angle = (length, horizontal, vertical)にしたもの
*/
void glArrow(double x0, double y0, double z0, double length, double horizontal, double vertical)
void glArrow(vector<double> pos0, vector<double> angle)

/*
 * (x0, y0)を中心に、x^2の係数がslopeで、xの区間[ex_range, range_x]の二時間数を作成
*/
void glCurve(double x0, double y0, double slope, double range_x, double ex_range)

/*
 * これの発展系のglCircle3Dを利用するのをお勧めする
 * (center_x, center_y, center_z)を中心に半径radiusの正多角形を作成
 * vertex: 何角形か、これを100くらいにするとほぼ円に見える
 * phase: 位相で、これにより円を回転することができる
 * rate: 0~1までで、多角形の周を途中で切ることができる
 * isfill: 内部を埋めるかどうか
*/
void glCircle(double center_x, double center_y, double center_z, double radius, double phase, int vertexes, double rate, bool isfill)

/*
 * require: CommonCoordSystem
 * glCircleが、任意の向きで描画できるようになったバージョン
 * 向きはオイラー角(x, y, z) = (vertical, 0, horizontal)となっている
*/
void glCircle3D(double x0, double y0, double z0, double radius, double horizontal, double vertical, double phase, int vertexes, double rate, bool isfill)

/*
 * require: CommonCoordSystem
 * らせんを作成
 * turns以外はglCircle3Dと同様
 * turns: らせんの巻き数を設定
*/
void glSpiral(double x0, double y0, double z0, double radius, double height, double horizontal, double vertical, double phase, int vertexes, double rate, bool isfill, double turns)

/*
 * require: CommonCoordSystem
 * うず巻きを作成
 * 引数はglCircle3Dと同様
 * turns: うずの巻き数を設定
*/
void glVortex3D(double x0, double y0, double z0, double radius, double height, double horizontal, double vertical, double phase, int vertexes, double rate, bool isfill, double turns)

/*
 * (0, 0, 0)を中心に3軸を作成
 * (x, y, z) = (Blue, Red, Green)で固定
 * 長さは適当に1000にしている
*/
void glAxis(double length = 1000)
```

## glArrow
* 使用ライブラリ: CoordSystem.h

矢印ってあったら楽だなーっていう感じで作りました。

## glPhysics
* 使用ライブラリ: CoordSystem.h

もとはCoordSystemでした。
これに加えていろいろな概念(力、熱、電磁気等)を追加したいです。(願望)

### Physics.h, Physics.cpp以外のファイル
* 使用ファイル(全て): CoordSystem.h, Physics.h

これらはサンプルです。
force.cppだけforce.cppなのは、力の概念を加えたときに作ったサンプルだからです。
* projectile_motion.cpp: 放物線運動
* circle_motion.cpp: 円運動
* force.cpp: 力の概念を利用した単振動

## loading
* 使用ライブラリ: jfont.h

ローディング画面(単体)です。
好きな文字を入れて好きなローディング画面を作ろう。

## glCircle
* 使用ライブラリ: CoordSystem.h

円を任意の法線ベクトル(水平角, 垂直角で定められたベクトル)を使って描くプログラムです。
ほら, 任意の角度でさ, 動くのってさ, 良いじゃん?

## glSpiral
* 使用ライブラリ: CoordSystem.h

glCircleのコードを少しいじった結果できた渦や螺旋です。なんと、今回は一気に3種も!

## glCube
* 使用ライブラリ: CoordSystem.h

既存の立方体関数をいつもも感じで作り直しました。
そういえば...glCubeに似た関数ってOpenGLライブラリに存在したような... ( https://w.atwiki.jp/opengl/pages/29.html )
