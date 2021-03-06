#ifndef _VERSION_H__INCLUDED_
#define _VERSION_H__INCLUDED_

//2005/11/10 v0.01
//2005/11/10 v0.02
//2005/11/10 v0.03
//2005/11/11 v0.04
//	RS232C通信エラーが多発するため、ためしに最適化を「実行速度」から「デフォルト」に変更
//2005/11/11 v0.05
//	RS232Cのタイムアウト設定を変更
//2005/11/11 v0.06
//	設定書き込み後のデータ受信フォーマットを変更
//2005/11/11 v0.07
//	シャッター設定のバグを修正
//2005/11/14 v0.08
//	設定書き込み後のデータ受信フォーマットチェック変更
//	(データ部長として00H/01H共に許容)
//2005/11/15 v0.09
//	シャッター設定にライン数・クロック数表示を追加
//	ショートジャンパー検査時に画像をまたがってショート検査を行うようにした
//-----------------------------------------------------------------------------
//2005/11/24 v0.10
//2005/11/24 v0.11
//2005/11/25 v0.12
//2005/11/25 v0.13
//2005/11/25 v0.14
//2005/11/28 v0.15
//2005/11/28 v0.16
//2005/11/28 v0.17
//2005/11/28 v0.18
//	[光軸検査]プレビュー画面上のクリックで位置を設定できるようにした
//2005/11/29 v0.19
//	表示されるメッセージを集約した
//2005/12/07 v0.20
//	検査時に通信で電源をONOFF
//	カメラバージョン検査追加
//	カラーバー検査追加(未判定)
//	画素欠陥検査追加
//	ゴミ検査に積算機能および平均最大最小表示機能を追加
//	入出力端子検査追加
//	光軸検査判定値変更(15%,2度),自動検出機能追加
//2005/12/09 v0.21
//	入出力端子検査修正
//2005/12/14 v0.22
//	画素欠陥検査：シャッター変更機能追加
//2005/12/15 v0.23
//	「周辺機器通信検査」「汎用ポート検査」をデフォルトで無効
//	チェックボックスのロック追加
//	カラー検査(元カラーバー検査)に判定を追加
//		BR比率：(B/Rx100)の最大最小
//		GrR比率：(Gr/Rx100)の最大最小
//		混色比率：((Gr-Gb)/(R-B)x100)の最大
//	画素欠陥検査：蓄積フレーム/ゲイン/シャッター変更機能および平均最大最小表示機能削除
//　ゴミ検査：蓄積機能および平均最大最小表示機能削除
//2005/12/16 v0.24
//	判定値初期値設定
//	光軸検査自動/リセットボタン削除
//	検査終了時メッセージから「電源をOFF」を削除
//	電源投入時待機処理追加
//	カラー検査の混色比率判定を絶対値に変更
//		混色比率：|((Gr-Gb)/(R-B)x100)|の最大

//2005/12/28 v0.25
//	カメラの入出力端子を使用した光源制御に対応
//		AUTO_CHANGE_LIGHT_MODEで切替
//			・画素欠陥検査「遮光してください」を削除
//			・チェックボックスの位置を検査の順番にあわせて変更「カラー検査」の位置
//	WaveFormを有効にした
//2005/12/29 v0.26
//	光軸検査の調整角度を0.5から0.1度単位に変更
//	光軸自動検査の位置を変更
//2005/12/29 v0.27 ゴミ検査にゴミ測定を追加
//2006/01/05 v0.28
//	ゴミ検査時の測定値を最大最小値から平均からの最大振れ幅へ変更
//	ゴミ検出位置を表示
//2006/01/05 v0.29
//	ゴミ検査時の蓄積フレーム数を4フレーム(2)から16フレーム(4)へ変更
//	ゴミ検査時のメッセージを変更
//	ゴミ検査のごみ用の閾値を70.0から100.0に変更
//2006/01/06 v0.30
//	『電源ノイズ検査』ボタンを追加
//	プレビュー画面が移動できなくなる現象を修正(MessageBoxをAfxMessageBoxに変更)
//2006/02/09 v0.31
//	ディジタルゲイン調整機能追加
//	各検査毎に検査日時をログに記録
//	検査初期値変更
//	シリアルID書込機能追加
//2006/02/14 v0.32
//	ディジタルゲイン0書込機能追加
//	EEPROM書込保護機能追加対応
//2006/02/20 v0.33
//	ショートジャンパーチェック異常時に終了しないバグを修正
//2006/03/03 v0.34
//	シリアルIDのEEPROM上のフォーマットを修正
//	ディジタルゲイン調整機能修正
//2006/04/14 v0.35
//	ハードウェアバージョンEEPROM書込機能追加

//2006/04/25 v0.36
//	入出力検査失敗時に正常終了してしまうバグを修正

//2006/06/30 v0.37 Beta01
//	不要なライブラリ(EasyXXX)とのリンクを削除
//	アナログ対応
//	白黒に対応
//	Euresysボードが複数存在する場合の処理を追加
//	機種別設定ファイル/機種別ログ保存機能追加
//　Euresysドライババージョンチェック機能追加
//	治具のポート番号指定機能追加
//	8ビット対応開始
//	HDVD出力検査対応開始
//	WaveFormのチラツキを改善
//2006/07/05 v0.37 Beta02
//2006/07/06 v0.37 Beta03
//	カメラファイルを読み替えるように修正
//	アナログのショートジャンパー検査追加
//	HDVD方向を通信で変更するように修正
//	EEPROMをバックアップする機能を追加
//	TRI初期ロット用に検査項目を無効化
//2006/07/07 18:46 v0.37 Beta04
//2006/07/07 v0.37 Beta05
//	Vリセットで処理するように修正
//	パーシャルはノンリセットで処理するように修正
//2006/07/10 9:57 v0.37 Beta06
//2006/07/10 v0.37 Beta07
//	アナログのパルス幅トリガ用のカメラファイルを追加
//2006/07/11 v0.37 Beta08
//	Corecoボード対応開始
//	8ビット用の検査初期値設定
//2006/07/26 v0.37 Beta09
//	シリアル番号をOMRONとSENTECHで変更
//2006/07/28 13:41 v0.37 Beta10
//	静止画取得時のイベントのリセットタイミングを修正
//	有効とする検査項目の状態をTRI初期ロット用のものから現時点でのアナログタイプ用のものに変更
//	ログにライブラリの下位4バイトの情報を追加(MultiCamのパッチ番号)
//　ボリュームチェックの順番を変更(「反時計→時計」から「時計→反時計」へ)
//2006/07/28 15:36 v0.37 Beta11
//	CameraLink時もトリガ用のカメラファイルに切り替える機能を追加
//2006/07/28 19:10 v0.37 Beta12
//	対応機種を追加
//2006/08/02 v0.37 Beta13
//	CCDロットNOの下4桁に数字を入力できるように変更
//	消費電流検査で通信エラー発生時にエラーメッセージが繰り返し表示される不具合修正
//	CorecoX64-ANで正常に動作するようになった(60FPSのみ)
//2006/08/03 v0.37 Beta14
//	検査前検査後(正常終了時のみ)にレジスタをEEPROMの値でリセットする機能を追加
//		工場の場合は電源が自動で落ちるため関係ない。
//		開発内で手動で設定した設定値や
//		検査が異常終了した場合に検査用に設定した値により
//		結果が変わってしまうのを防ぐため
//2006/08/04 v0.37 Beta15
//	カメラリンクおよびOMRON専用のメッセージを修正
//	HDVD入出力/終端抵抗の手動検査に対応
//	デフォルト無効としていた検査項目を有効に変更
//2006/08/07 v0.37 Beta16
//	画素欠検討中
//	Euresysアナログボードのゲイン設定
//2006/08/08 v0.37 Beta17->正式版
//	CLC33B,A83の検査基準値更新
//	画素欠検査を出荷時のゲイン設定で行うように変更
//	ゴミ検査NG時に検査が続行されてしまう不具合を修正
//2006/08/09
//	A33(60)のカメラファイルのタイミングを修正
//	(依然として左に1画素ブランキングが入る)
//2006/08/10 v0.37
//	ゴミチェックを平均との差ではなく平均との差の割合で判断する機能を追加(ENABLE_DUST_THRESHOLD_RATIOを有効にする)
//	ゴミ検査で画素値が一定の時に平均値等が表示されない不具合を修正
//2006/08/10 v0.38 Beta1 ゴミ検査の閾値をピクセル値から変動の割合へ変更
//2006/08/22 v0.38 Beta2 プレビューウィンドウをメイン画面の後方に移動できるように変更
//2006/09/04 v0.38 Beta2 ゴミ検査時の検査領域から上下左右の指定画素を除外できるように変更
//2006/09/05 v0.38 Beta3 ショートジャンパー検査でシャッター速度も振るように変更
//2006/09/06 v0.38 Beta4 HDVD検査にボードのHDVD出力を使用するように変更
//2006/09/07 v0.38 正式版 (検査開始／終了時のEEPROM読み込みから先頭の4バイトを除外)
//2006/09/11 v0.39 Beta1 HDVD検査にボードのHDVD出力を使用するように変更した内容を有効化
//2006/09/11 v0.39 Beta1 ゴミ検査の閾値の初期値を10%から20%(33系)/30%(83系)に変更
//2006/09/20 v0.39 Beta2 ショートジャンパー検査でシャッターの値をデフォルトに戻していないバグを修正
//2006/09/20 v0.39 Beta3 ゴミ検査の有効な明るさ範囲を700-800へ変更
//2006/09/21 v0.39 Beta4 E42,E43対応
//2006/09/21 v0.39 正式版
//2006/10/11 v0.40 Beta1 E42,E43初期値変更
//2006/10/11 v0.40 Beta1 CCDロット番号の全桁を英数字に変更
//2006/10/11 v0.40 Beta1 外部同期時にシリアル通信を行わないように変更
//2006/10/11 v0.40 Beta2 初期値変更
//2006/10/11 v0.40 正式版 電源投入後一定時間待機するように変更(E42,43で検査開始時に通信エラーが発生したため)
//2006/10/12 v0.40 正式版 初期値変更
//2006/10/12 v0.41 正式版 初期値変更(E42-映像出力レベル800±20->815±15)
//2006/10/13 v0.42 Beta1 DAC/ディジタルゲイン調整エリア指定対応
//2006/11/08 v0.42 Beta2 SN検査時ゲイン初期値変更
//2006/11/15 v0.42 Beta3 ショートジャンパー検査時の露光時間をさらに短く振るように修正(STC-CL83Aで失敗したため)
//2006/11/15 v0.42 Beta3 バージョン検査初期値変更
//2006/11/15 v0.42 正式版 Beta1での変更は下記の定数をコメントし無効化している
//								#define ENABLE_DAC_CHECK_SET_AREA
//								#define ENABLE_DIGITAL_GAIN_CHECK_SET_AREA
//2006/12/26 v0.43 Beta1 出荷検査モードを追加
//2006/12/26 v0.43 Beta1 ハードウェアバージョン書き込み位置変更
//2006/12/26 v0.43 Beta1 カラー検査初期値変更
//2006/01/18 v0.43 Beta2 検査ごとの初期値わけに対応
//2006/01/18 v0.43 正式版
//2007/03/02 v0.44 Beta1 Bシリーズの汎用端子を使用したI2C通信チェックを追加
//2007/03/05 v0.44 Beta2 治具のI2Cスレーブアドレスを設定できるように変更
//2007/03/06 v0.44 Beta3 カメラからの受信コードの扱いを変更
//2007/03/07 v0.44 Beta4 カメラからの受信異常エラー(0x10)発生時に数回リトライするように変更
//2007/03/07 v0.44 Beta4 検査初期値変更
//						CLC33B,CL33B:ファームバージョン0x121C -> 0x130C
//						CLC33A,CLC33B:カラー検査MinGRRRatio = 6.0 -> 4.0
//2007/03/07 v0.44 Beta4 治具のバージョン情報取得関数を追加
//2007/03/08 v0.44 Beta5 I2C検査回数のデフォルト値を600に変更
//2007/03/08 v0.44 正式版
//2007/03/27 v0.45 Beta1 CL202,CLC202対応
//2007/03/29 v0.45 Beta2 ゴミ検査時のオートスクロール対応
//2007/03/29 v0.45 Beta2 プレビューウィンドウを画面内に収めるモードを追加
//2007/04/04 v0.45 Beta3 不要なスクロールバー表示を抑制
//2007/04/23 v0.45 Beta4 A33D(for ASM)を追加
//2007/04/25 v0.45 Beta5 A33D[90FPS]検査終了時ブルーバック対処[画像取得を停止してからFPSを元に戻す]
//2007/04/26 v0.45 Beta5 A33D[90FPS](60)を追加
//2007/04/26 v0.45 Beta5 A33D[60FPS](60)のバージョン初期値を修正
//2007/04/27 v0.45 Beta6 CL202/CLC202用の規格を設定
//2007/04/27 v0.45 正式版
//2007/05/15 v0.46 Beta1 アナログカメラのJITTER検査追加
//2007/05/17 v0.46 Beta2 検査項目すべてチェック機能追加
//2007/05/17 v0.46 Beta2 JITTER検査でテストパターンを使用するように変更
//2007/05/18 v0.46 Beta3 オフセットを動的に設定できる機能を追加
//2007/05/22 v0.46 Beta4 シャッターゲイン更新時の待機フレーム数をINIファイルで設定できるように変更
//2007/05/25 v0.46 Beta5 色差評価機能追加
//2007/05/28 v0.46 Beta6 IOTAである程度動作するように修正
//2007/06/01 v0.46 Beta6 ジッタ検査変更(実行位置、初期値、待機時間)
//2007/06/01 v0.46 Beta7 スクロールバーが消える不具合対処
//2007/06/01 v0.46 正式版
//2007/06/05 v0.47 Beta1 A33Dのカメラファイルを変更
//							有効ラインが1ライン少ない(493)ため、2ライン減らし494とした
//							ジッタ検査用のカメラファイルの画像サイズが他のファイルと異なっていたため修正
//2007/06/05 v0.47 Beta1 スクロールバーがないときにゴミ検査で画面がスクロールされてしまうバグを修正
//2007/06/06 v0.47 Beta2 A152対応
//2007/06/06 v0.47 Beta2 Grab開始または終了時に失敗した場合はリトライするように変更
//2007/06/12 v0.47 Beta3 CL152,CLC152対応
//2007/06/14 v0.47 Beta4 色差のオフセット値を修正
//2007/06/14 v0.47 Beta4 色差データスナップ取得機能追加
//2007/06/15 v0.47 Beta5 色差画面にソフトトリガボタンを追加
//2007/06/15 v0.47 Beta5 ウェーブフォームの初期ライン数をAll0から0,中央,下端に変更
//2007/06/15 v0.47 Beta5 カメラの通信レートをINIファイルへ保存
//2007/06/21 v0.47 Beta6 CLC152B(15FPS/9600bps)対応
//2007/07/03 v0.47 Beta7 CLC232B対応
//2007/07/04 v0.47 Beta7 画素欠陥検査で設定値が読み込まれないバグを修正
//2007/07/05 v0.47 Beta7 CLC232B対応(ディジタルゲイン調整：オフセットのみを調整)
//2007/07/06 v0.47 Beta8 CLC232B対応(ショートジャンパーチェック2TAP対応)
//2007/07/13 v0.47 Beta9 CLC232B対応(電源の最大電流値を400mAから500mAに変更)
//2007/07/20 v0.47 Beta10 光軸検査：1度位置を変更するまで[OK]ボタンを押せないように変更
//2007/08/21 v0.47 Beta11 STC-E42F追加
//2007/08/21 v0.47 正式版
//2007/08/22 v0.48 Beta1 ゲイン調整時のシャッターを設定できるように変更
//2007/09/06 v0.48 Beta2 縦縞検査機能を追加
//2007/09/11 v0.48 Beta3 A33Dシリーズ用マニュアルゲイン調整機能を追加
//2007/09/11 v0.48 Beta4 縦縞検査を画素欠検査の後に移動
//2007/09/11 v0.48 Beta4 縦縞検査のFF設定初期値を0に変更
//2007/09/11 v0.48 Beta4 縦縞検査のFF設定が元に戻っていない不具合を修正
//2007/09/12 v0.48 Beta5 A33Dシリーズ用マニュアルゲイン調整の明るさ表示を間引き処理
//2007/09/13 v0.48 Beta6 A33Dシリーズ用マニュアルゲイン調整時のシャッターを設定できるように変更
//2007/09/18 v0.48 Beta7 設定値を変更
//2007/09/18 v0.48 正式版
//2007/09/27 v0.49 Beta1 作業者入力機能を追加
//2007/09/27 v0.49 Beta1 規格値変更
//2007/09/27 v0.49 Beta02 マニュアルゲインボリューム検査の出荷検査規格値の設定ファイル内の名称を変更
//2007/09/27 v0.49 Beta02 HXA規格値変更
//2007/09/27 v0.49 正式版
//2007/10/10 v0.50 Beta01 C420対応
//2007/10/13 v0.50 Beta02 C420高速パルス調整
//2007/10/15 v0.50 Beta03 リストをクリップボードへコピーする機能を追加
//2007/10/15 v0.50 Beta03 メインウィンドウの再描画を検討
//2007/10/16 v0.50 Beta03 C420高速パルス調整にADCKを追加
//2007/10/17 v0.50 Beta03 C420検査開始時にフリーラン/トリガ切り替えを自動に設定
//2007/12/05 v0.50 Beta04 C420検査時に「ディジタルゲイン調整」「ジッタ検査」「光軸検査」を除外
//2007/12/05 v0.50 Beta04 C420 画素欠検査時にユーザゲイン1を使用するように変更
//2007/12/05 v0.50 Beta04 C420 映像出力検査時のゲインをユーザゲイン1に変更
//2007/12/05 v0.50 Beta04 C420 ゲイン調整を高速化
//2007/12/05 v0.50 Beta05 C420 ゲイン調整を高速化
//2007/12/11 v0.50 Beta06 C420 メッセージ変更
//2007/12/12 v0.50 Beta07 C420 メッセージ変更
//2007/12/14 v0.50 Beta08 C420をC420Tに変更
//2007/12/14 v0.50 Beta08 C420Tの検査基準値を変更
//2007/12/14 v0.50 正式版 C420Tの検査基準値を変更
//2007/12/17 v0.51 Beta01 C420T ゲイン検査時のシャッターを設定できるように変更
//2007/12/17 v0.51 Beta01 C420T SN検査のゲイン値をユーザゲイン1に変更
//2007/12/17 v0.51 Beta01 C420T SN検査のシャッター値を7500に変更
//2007/12/17 v0.51 Beta02 C420T ゲイン検査時のシャッター設定が反映されない不具合を修正
//2007/12/17 v0.51 正式版
//2007/12/19 v0.52 Beta01 HE42A対応
//2007/12/20 v0.52 Beta02 STC-A152の検査基準値を変更
//2007/12/21 v0.52 Beta03 STC-HE42の検査基準値を変更
//2007/12/21 v0.52 正式版
//2007/12/26 v0.53 Beta01 STC-A152の検査基準値を変更
//2007/12/26 v0.53 Beta01 STC-CL202, CLC202の光軸検査基準値を変更
//2007/12/26 v0.53 Beta01 ゴミ検査基準値を変更
//2008/02/06 v0.53 Beta02 STC-CL232A, STC-CLC232A対応 
//2008/02/14 v0.53 Beta03 STC-C42A, STC-C43A対応
//2008/02/20 v0.53 Beta04 縦縞検査の結果をログに追加
//2008/02/20 v0.53 Beta04 検査基準値を変更
//2008/02/20 v0.53 Beta05 検査基準値を変更
//2008/02/20 v0.53 Beta06 検査基準値を変更
//2008/02/20 v0.53 正式版
//2008/02/21 v0.54 Beta01 A33MEW, E43MEW対応
//2008/02/26 v0.54 Beta02 A202A対応
//2008/02/28 v0.54 Beta03 A33MEW, E43MEW対応
//2008/02/28 v0.54 Beta03 PixelMixの8ビット用の検査基準値を整理
//2008/02/28 v0.54 Beta04 シャッター検査の閾値をINIファイルへ追加
//2008/02/29 v0.54 Beta05 A33MEW, E43MEW対応
//2008/02/29 v0.54 Beta05 電源ONの順番を「治具->カメラ」、電源OFFの順番を「カメラ->治具」に変更
//2008/03/05 v0.54 Beta06 カメラのボーレートをB以外9600bpsへ変更
//2008/03/05 v0.54 Beta06 規格値変更
//2008/03/05 v0.54  正式版
//2008/03/05 v0.55 Beta01 A33MEW, E43MEW対応
//2008/03/18 v0.55 Beta02 規格値変更
//2008/03/24 v0.55 Beta03 ゴミ検査時に明るすぎると指示がおかしくなる不具合を修正
//2008/04/03 v0.55 Beta03 STC-A83A(44), STC-A83D(43)対応
//2008/04/03 v0.55 Beta03 STC-CL83A(44), STC-CLC83A(44)対応
//2008/04/04 v0.55 Beta04 タイミング調整をC420以外でも動作するようにロックをはずした(通信コマンドが対応しないカメラでは動作しない)
//2008/04/07 v0.55 Beta05 規格値変更
//2008/04/07 v0.55 Beta05 STC-CLC83D(29)対応
//2008/04/18 v0.55 Beta06 STC-152シリーズ FPS切り替え対応
//2008/04/18 v0.55 Beta06 規格値変更
//2008/05/08 v0.55 Beta07 規格値変更
//2008/05/09 v0.55 Beta08 出荷検査の対象を変更
//2008/05/09 v0.55 Beta09 規格値変更
//2008/05/09 v0.55 Beta09 出荷検査の対象を変更
//2008/05/09 v0.55  正式版
//2008/05/14 v0.56 Beta01 出荷検査の対象を再変更
//2008/05/14 v0.56 Beta01 規格値変更
//2008/05/14 v0.56 Beta01 CL202B, CLC202Bを削除
//2008/05/14 v0.56 Beta02 規格値変更
//2008/05/14 v0.56 正式版
//2008/06/06 v0.57 Beta01 STC-RXXBシリーズ対応
//2008/06/06 v0.57 Beta01 規格値変更
//2008/06/23 v0.57 Beta02 異常終了時に検査ログファイルの名前を変更
//2008/07/01 v0.57 Beta03 規格値変更
//2008/07/01 v0.57 Beta04 規格値変更
//2008/07/02 v0.57 正式版
//2008/07/30 v0.58 Beta01 STC-RXXB対応
//2008/07/30 v0.58 Beta01 映像確認ボタン追加
//2008/07/31 v0.58 Beta02 STC-RXXBショートジャンパー検査対応
//2008/08/04 v0.58 Beta03 規格値変更
//2008/08/18 v0.58 Beta04 メッセージ変更
//2008/08/19 v0.58 Beta05 規格値変更
//2008/08/19 v0.58 Beta05 メッセージ変更
//2008/08/19 v0.58 Beta06 メッセージ変更
//2008/08/21 v0.58 正式版
//2008/09/12 v0.59 Beta01 STC-CLX202Gシリーズ対応
//2008/09/12 v0.59 Beta01 STC-CLX202Gシリーズ用画素欠検査対応
//2008/09/24 v0.59 Beta02 STC-C420規格値変更
//2008/09/25 v0.59 Beta03 起動時に設定ファイルを削除
//2008/10/01 v0.59 Beta04 検査対象の機種や検査有効無効切替を検査側で判別するためのベースを作成
//2008/10/01 v0.59 Beta04 上下反転検査を追加
//2008/10/01 v0.59 Beta04 検査開始部分の共通化
//2008/10/01 v0.59 Beta05 ビニング検査を追加
//2008/10/07 v0.59 Beta06 規格値変更
//2008/10/07 v0.59 Beta07 規格値変更
//2008/10/07 v0.59 正式版
//2008/10/15 v0.60 Beta01 CL202G, CLC202G外部同期検査対応
//2008/10/15 v0.60 Beta02 Beta01の設定を5から4に変更
//2008/10/15 v0.60 Beta03 Beta01のメッセージを変更
//2008/10/15 v0.60 Beta03 規格値変更
//2008/10/15 v0.60 正式版

//2008/10/28 v0.61 Beta01 C420N対応
//2008/10/28 v0.61 Beta01 規格値変更
//2008/11/05 v0.61 正式版
//2008/11/12 v0.62 Beta01 規格値変更
//2008/11/12 v0.62 Beta01 CL202H対応
//2008/11/12 v0.62 Beta01 機種コードから各種コードを取得するマクロを追加
//2008/11/13 v0.62 Beta02 機種コードから各種コードを取得するマクロを修正
//2008/11/14 v0.62 正式版
//2008/11/18 v0.63 Beta01 バージョン検査の変数初期化漏れ
//2008/12/03 v0.63 Beta02 CL33B,CL232Bにビニング検査を追加 
//2008/12/03 v0.63 Beta02 検査時のボーレートをログファイルに保存
//2008/12/03 v0.63 Beta02 表示されるカメラ名にO社型番を追加
//2008/12/03 v0.63 Beta02 ミスミ用機種追加(標準品との違いはボーレートのみ)
//2008/12/04 v0.63 Beta03 デジタルゲイン調整時の破棄フレーム数を変更
//2008/12/04 v0.63 Beta03 CL232Bのビニング検査時の画像の高さを設定
//2008/12/04 v0.63 Beta03 デジタルゲインの検査範囲を制限
//2008/12/04 v0.63 Beta03 デジタルゲイン検査のゲイン設定部を整理
//2008/12/10 v0.63 Beta04 CL232B(HXA)のビニング検査無効(FPGAに不具合があるため)
//2008/12/10 v0.63 Beta04 ミスミ用機種追加(標準品の違いはボーレートおよび33のフレームレート)
//2008/12/10 v0.63 Beta04 フレームレート検査を追加
//2008/12/26 v0.63 Beta05 ミスミ用機種ボーレート修正
//2008/12/26 v0.63 Beta05 規格値変更
//2009/01/05 v0.63 正式版
//2009/01/23 v0.64 Beta01 Value以外のボードに対応できるように変更
//2009/01/26 v0.64 Beta01 異なるカメラリンクボードに同じCOMポートが割り当てられてしまうMultiCam不具合対処
//2009/03/03 v0.64 Beta02 規格値変更
//2009/03/03 v0.64 Beta02 プロダクト番号QRコード入力対応検討
//2009/03/03 v0.64 Beta02 プロダクト番号の年号を100年分表示するように変更
//2009/03/04 v0.64 Beta03 ボード選択不具合修正
//2009/03/04 v0.64 Beta04 規格値修正
//2009/03/04 v0.64 正式版 QRコード入力用のテキストボックスを無効に変更
//2009/03/16 v0.65 Beta01 QRコード入力用のテキストボックスを有効に変更
//2009/03/16 v0.65 Beta01 認識後QRコード入力用のテキストボックスをクリア
//							->クリアするとシリアル番号以外の文字列をご認識してしまうため取りやめ
//2009/03/16 v0.65 Beta01 シリアル番号のコンボボックスの初期状態変更
//2009/03/24 v0.65 Beta02 規格値変更
//2009/03/24 v0.65 Beta02 シリアル番号のコンボボックスの初期状態変更(オムロン用)
//2009/03/24 v0.65 Beta03 オムロン時QRコード用のテキストボックスを無効化
//2009/03/25 v0.65 正式版

//	同じ画像サイズのカメラを検査する前にデスクトップ表示でプレビューウィンドウを隠すと
//	スクロールバーが消える。
//	その後検査を開始してもウィンドウサイズに変更がないためスクロールバーが現れない。

//#define BETA_STRING TEXT(" Beta03")
class CVersion
{
private:
	/*
#ifdef _UNICODE
	USHORT	m_szModuleFileName[MAX_PATH];
#else
	CHAR	m_szModuleFileName[MAX_PATH];
#endif*/
	TCHAR		m_szModuleFileName[MAX_PATH];
	HINSTANCE m_hInstance;
	BOOL	m_bGotVersionDataFg;
	DWORD	m_dwProductVersionMS;
	DWORD	m_dwProductVersionLS;

	BOOL	mbGetVersionInfo(void);
public:
	CVersion(HINSTANCE hInstance);
	~CVersion();

	BOOL bGetVersion(PDWORD pdwProductVersionMS,PDWORD pdwProductVersionLS);
};
#endif

