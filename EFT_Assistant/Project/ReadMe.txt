EFT Assistant　ver0.3.5　　　　　　作成者　EMIYA_UBW

このアプリケーションについて
　　Escape From Tarkovのアイテム売買、タスククリア、ハイドアウト解放などをアシストするオーバーレイツールになります。

操作
　　共通操作
　　　　・マウス左ボタン　　　　　　　ボタンクリック操作
　　　　・右ボタンドラッグ　　　　　　閲覧個所の移動
　　　　・マウスホイールスクロール　　閲覧個所のズーム
　　特殊操作
　　　　・マウス左ボタンドラッグ　　　マップ画面　横方向でマップ回転
　　　　・マウスホイールスクロール　　リスト画面　リストのスクロール
　　上タイトル部分
　　　　・マウスホイールドラッグ　　　ウィンドウの位置を移動
　　　　・折りたたむ　　　　　　　　　最小化/展開
　　　　・すべてのウィンドウを閉じる　アイテム画面などの表示をすべて閉じる
　　　　・ログを表示　　　　　　　　　読み込みエラー等の表示/非表示

実装
　　基本仕様関連
　　　　・スタンドアローン、オーバーレイ表示で動作するC++アプリケーション
　　　　・tarkov.dev( https://tarkov.dev/api/ )のAPIから最新の情報を取得
　　　　・apiの情報を基に集める必要のあるアイテムのリストを形成
　　　　・ついでにマップ(タスクで訪れる場所のガイド、簡易測距儀機能付き)などのレイド中お役立ち機能を搭載
　　カスタム
　　　　・銃器カスタマイズ数値確認機能
　　アイテム
　　　　・売却価格のチェック
　　　　・検索追加
　　アイテムリスト
　　　　・タスクで納品が必要なアイテム一覧
　　　　・ハイドアウト解放に必要なアイテム一覧
　　　　・上記をクラフト/購入するのに必要なアイテム一覧
　　　　・売るしか意味のないアイテム一覧
　　タスク
　　　　・タスク条件などをリスト/ツリー状に閲覧
　　　　・タスク完了、目標レベルに応じた表示フィルター
　　ハイドアウト
　　　　・解放条件などをリスト/ツリー状に閲覧
　　　　・解放済などに応じた表示フィルター
　　マップ
　　　　・距離目安円(2Dマップでのみ)
　　　　・タスク回収品の場所ガイド
　　トレーダー
　　　　・購入価格のチェック
　　APIからデータ更新
　　　　・最新の情報を取得
　　バグ
　　　　（既知かつ未対応のバグなし）
未達/実装予定
　　基本仕様関連
　　　　・ウィンドウ版の開始オプション追加
　　カスタム
　　　　・カスタマイズ最適化(目標エルゴ、目標縦リコイルに合わせたカスタマイズ候補を表示)
　　タスク
　　　　・タスクの種類早見
　　マップ
　　　　・タスク回収品の場所ガイド(残り　手動なので確認しきれないかも)
　　　　・その地域の写真
　　ハイドアウト
　　　　・コスパ計算
　　　　・ハイドアウトで作ったはいいが何から使えばいいのか
　　その他
　　　　・Scavの服早見表
　　　　・アイテムなどの日本語説明追加


履歴
2023/01/08 ver0.0.1公開(プレパーのタスクデータ処理機能)
2023/01/09 ver0.0.2公開(バグ修正)
2023/01/10 ver0.0.3公開(タイトル追加、アイテムイメージ表示追加)
2023/01/11 ver0.0.4公開(アイテム画像追加)
2023/01/12 ver0.0.5公開(ABCDEの鍵アイテムを追加)
2023/01/14 ver0.0.6公開(F～Zの鍵アイテムを追加、空ける場所がない鍵のソート機能追加)
2023/01/14 ver0.0.7公開(セラピストのタスクデータ追加)
2023/01/15 ver0.0.8公開(アイテム絞り込み処理変更)
2023/01/17 ver0.0.10公開(スキヤ対応)
2023/01/18 ver0.0.11公開(全アーマーに対応、カスタムのマップ仮追加)
2023/01/21 ver0.0.12公開(マップの基本機能実装)
2023/01/21 ver0.1.0公開(描画システム改修)
2023/01/23 ver0.1.1公開(アイテムに装着できるアイテム確認に対応,ADARと6B47に対応)
2023/01/28 ver0.1.2公開(武器パーツの基礎入力)
2023/02/04 ver0.1.3公開(カスタマイズ機能追加(M4A1,ADAR,TX-15))
2023/02/11 ver0.1.4公開(カスタマイズ機能に使用パーツのロック機能追加)
2023/02/20 ver0.1.5公開(カスタマイズ機能調整、ガンスミス用パーツ全実装)
2023/02/25 ver0.1.6公開(ピースキーパータスク追加、複数条件のタスク反映、軽量化)
2023/03/02 ver0.1.7a公開(ルート品の値段、マス単価表示追加)
2023/03/02 ver0.1.7b公開(値段更新を試験追加)
2023/03/12 ver0.1.8公開(タスクのAPI取得機能を追加、すべてのタスク実装)
2023/03/28 ver0.2.0公開(タスクで必要なアイテムかどうかのチェック機能追加)
2023/04/01 ver0.2.1公開(APIからカスタマイズ周りの情報が取れるよう機能拡張:アイテム周りを手動更新する必要がなくなりました)
2023/04/03 ver0.2.2公開(アイテムの納品リスト追加)
2023/04/08 ver0.2.3公開(マップに測距/ピン追加)
2023/05/14 ver0.2.4公開(ハイドアウト機能　クラフトリスト閲覧機能)
2023/05/20 ver0.3.0公開(ハイドアウト機能完了　タスクに必要なアイテムリスト追加)
2023/05/21 ver0.3.1公開(ハイドアウトに必要なアイテムリスト追加　プレイヤーデータの追加)
2023/05/27 ver0.3.2公開(トレーダー交換(購入含む)の追加)
2023/06/02 ver0.3.3公開(読み込み時間短縮、リファクタリング)
2023/06/10 ver0.3.4公開(リファクタリング、エラー修正)
2023/06/12 ver0.3.5公開(アイテム検索の追加(アルファベットのみ))

以下著作権表記です

CC BY-NC-SA 3.0(公式wikiに準じます)

以下DXライブラリ著作権表記です

    　libjpeg　Copyright (C) 1991-2013, Thomas G. Lane, Guido Vollbeding.
　　　this software is based in part on the work of the Independent JPEG Group


　　　libpng　Copyright (C) 2004, 2006-2012 Glenn Randers-Pehrson.
　　　zlib　Copyright (C) 1995-2012 Jean-loup Gailly and Mark Adler.


　　　libtiff　Copyright (c) 1988-1997 Sam Leffler
　　　libtiff　Copyright (c) 1991-1997 Silicon Graphics, Inc.

　　　Permission to use, copy, modify, distribute, and sell this software and
　　　its documentation for any purpose is hereby granted without fee, provided
　　　that (i) the above copyright notices and this permission notice appear in
　　　all copies of the software and related documentation, and (ii) the names of
　　　Sam Leffler and Silicon Graphics may not be used in any advertising or
　　　publicity relating to the software without the specific, prior written
　　　permission of Sam Leffler and Silicon Graphics.

　　　THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
　　　EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
　　　WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.

　　　IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
　　　ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
　　　OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
　　　WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
　　　LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
　　　OF THIS SOFTWARE.


　　　libogg　Copyright (C) 2002-2009 Xiph.org Foundation

　　　Redistribution and use in source and binary forms, with or without
　　　modification, are permitted provided that the following conditions
　　　are met:

　　　- Redistributions of source code must retain the above copyright
　　　notice, this list of conditions and the following disclaimer.

　　　- Redistributions in binary form must reproduce the above copyright
　　　notice, this list of conditions and the following disclaimer in the
　　　documentation and/or other materials provided with the distribution.

　　　- Neither the name of the Xiph.org Foundation nor the names of its
　　　contributors may be used to endorse or promote products derived from
　　　this software without specific prior written permission.

　　　THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
　　　``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
　　　LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
　　　A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE FOUNDATION
　　　OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
　　　SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
　　　LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
　　　DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
　　　THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
　　　(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
　　　OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


　　　Opus audio codec
　　　Copyright 2001-2011 Xiph.Org, Skype Limited, Octasic,
　　　 Jean-Marc Valin, Timothy B. Terriberry,
　　　 CSIRO, Gregory Maxwell, Mark Borgerding,
　　　 Erik de Castro Lopo

　　　Redistribution and use in source and binary forms, with or without
　　　modification, are permitted provided that the following conditions
　　　are met:

　　　- Redistributions of source code must retain the above copyright
　　　notice, this list of conditions and the following disclaimer.

　　　- Redistributions in binary form must reproduce the above copyright
　　　notice, this list of conditions and the following disclaimer in the
　　　documentation and/or other materials provided with the distribution.

　　　- Neither the name of Internet Society, IETF or IETF Trust, nor the
　　　names of specific contributors, may be used to endorse or promote
　　　products derived from this software without specific prior written
　　　permission.

　　　THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
　　　``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
　　　LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
　　　A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
　　　OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
　　　EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
　　　PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
　　　PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
　　　LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
　　　NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
　　　SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


　　　Opusfile
　　　Copyright (c) 1994-2013 Xiph.Org Foundation and contributors

　　　Redistribution and use in source and binary forms, with or without
　　　modification, are permitted provided that the following conditions
　　　are met:

　　　- Redistributions of source code must retain the above copyright
　　　notice, this list of conditions and the following disclaimer.

　　　- Redistributions in binary form must reproduce the above copyright
　　　notice, this list of conditions and the following disclaimer in the
　　　documentation and/or other materials provided with the distribution.

　　　- Neither the name of the Xiph.Org Foundation nor the names of its
　　　contributors may be used to endorse or promote products derived from
　　　this software without specific prior written permission.

　　　THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
　　　``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
　　　LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
　　　A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE FOUNDATION
　　　OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
　　　SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
　　　LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
　　　DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
　　　THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
　　　(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
　　　OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


　　　Mersenne Twister
　　　Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
　　　All rights reserved.

　　　Redistribution and use in source and binary forms, with or without
　　　modification, are permitted provided that the following conditions
　　　are met:

　　　1. Redistributions of source code must retain the above copyright
　　　notice, this list of conditions and the following disclaimer.

　　　2. Redistributions in binary form must reproduce the above copyright
　　　notice, this list of conditions and the following disclaimer in the
　　　documentation and/or other materials provided with the distribution.

　　　3. The name of the author may not be used to endorse or promote products
　　　derived from this software without specific prior written permission.

　　　THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
　　　IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
　　　OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
　　　IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
　　　INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
　　　NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
　　　DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
　　　THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
　　　(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
　　　THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


　　　Bullet　Copyright (c) 2003-2006 Erwin Coumans.




Copyright c 2013-2022 Niels Lohmann

	Permission is hereby granted, free of charge, to any person obtaining a 
	copy of this software and associated documentation files (the “Software”), 
	to deal in the Software without restriction, including without limitation 
	the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.

