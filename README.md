# Super_Catch_Ball
## 制作時に追加したファイル
- scripts/Tasksフォルダ内の全てのファイル
- scripts/Bases/GameBase(.h .cpp)
- scripts/Engine/MyLibrary(.h .cpp)
## 内容を変更したファイル
### scripts/Engine/MyPG
- カメラの設定追加
- ゲーム全体で使用する変数追加
- システム関係のメソッド追加
- ゲームデータの読み込み・保存をするメソッド追加
- デバッグ用メソッド追加
### scripts/Engine/myGame
- 初期化データを格納するクラスを制作
## dataフォルダ概要
### color
パーティクル用の画像、モデルが入ったフォルダ
### debug
デバッグ用メソッドから出力された情報を記録するフォルダ
### gamedata
プレイヤーの移動速度や的の出現場所などのパラメータを設定するフォルダ
### image
ゲームで使用する2D画像が入ったフォルダ
### mesh
ゲームで使用する3Dメッシュが入ったフォルダ
## 特に力を入れた、もしくは工夫したファイル
- scripts/Engine/MyLibrary
- scripts/Bases/GameBase
- scripts/Tasks/Scene_MainGame/Task_PlayerBase
- 初期化データ読み込み処理（「scripts/Tasks」内のファイルで使用）

# 的配置ツール
## 制作時に追加したファイル
- Toolscripts/Tasks/CreatorTaskフォルダ内の全てのファイル
## 内容を変更したファイル
### Toolscripts/Tasks/GameManager
- プレイヤーや場外判定など、不要なオブジェクトを生成しないように変更
- クリエイターオブジェクト生成処理追加
### Toolscripts/Tasks/Task_Target
- ツール用の変数やメソッドの追加
## Tooldataフォルダ概要
### generateData
ツールで保存した情報を入れるフォルダ。生成した的の配置を合成するソフトが入っている。
#### その他のフォルダの概要はdataフォルダと同一
