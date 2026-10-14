# The Stations (ザ・ステーションズ) - 総合開発仕様 & 製品版 C++20 コアエンジン実装報告書

【取扱注意】 Level 5 Top Secret ※本ドキュメントは「The Stations」プロジェクトの製品版C++20コアエンジン実装および構築仕様に関する説明書です。

---

## 1. プロジェクト概要 & アーキテクチャ構成

本作「The Stations」は、「日常にある何の変哲もない無人駅」×「時空のねじれ」を一人称視点（1P）で体験するリアルホラーゲームです。
HUD非表示（0%）による没入感と、プレイヤーの生体＆行動データ（恐怖度スコア）を用いたプロシージャルレベル生成、時空の不可逆性（ドアを閉めると元の空間がメモリから破棄され絶対に戻れない構造）、EOS（Epic Online Services）を用いた全プラットフォーム（Windows / macOS / Android）クロスプレイ対応を特徴としています。

### ディレクトリ構成 & ビルド成果物配置 (Release/)
```
TheStations/
├── CMakeLists.txt                 # C++20 クロスプラットフォームビルドスクリプト (Release/ 出力パス強制設定)
├── Release/                       # 【厳格ビルド成果物出力ディレクトリ】
│   ├── windows/                   # TheStations.exe (Windows 64bit)
│   ├── mac/                       # TheStationsApp (macOS Mach-O バンドル)
│   └── android/                   # TheStations (Android NDK バイナリ)
├── src/
│   ├── main.cpp                   # メインエントリポイント
│   ├── shared/                    # 全OS共通共有コアエンジン logic
│   │   ├── types.hpp              # 共通型定義
│   │   ├── platform_interface.hpp # OS抽象化インターフェース
│   │   ├── graphics/              # プロシージャルテクスチャ生成 (コンクリート汚れ・血痕タイル・フィルムグレイン)
│   │   ├── audio/                 # プロシージャルオーディオDSP合成 (電車ドップラー・風のうねり・アナウンスチャイムPCM)
│   │   ├── ui/                    # UI / 画面ステートマシン (HUDレス設計)
│   │   ├── level/                 # プロシージャルレベル生成 & 空間不可逆性ロジック (50種ステーション)
│   │   ├── biometric/             # 生体認証 & 行動DDAアナライザ (ローカルRAM完結)
│   │   ├── network/               # EOS P2P ネットワーク & 5桁ロビーマッチング
│   │   ├── entity/                # エンティティAI & 10分間隠しタイマー & アナウンスギミック
│   │   └── anticheat/             # FIPS 180-2 SHA256 バイナリ検証 & 海賊版報復トラップエンジン (案B/案C)
│   ├── windows/                   # Windows native
│   ├── mac/                       # macOS native
│   └── android/                   # Android native
├── tests/                         # 厳格ユニットテストスイート (CTest)
│   ├── test_sanity.cpp
│   ├── test_ui.cpp
│   ├── test_level.cpp             # レベル生成決定性＆空間破棄検証
│   ├── test_biometric.cpp         # DDA権限拒否フォールバック検証
│   ├── test_eos.cpp               # 5桁ルームコード衝突耐性＆P2P検証
│   ├── test_entity.cpp            # AI状態遷移＆10分タイマー検証
│   └── test_anticheat.cpp         # 改ざん検知＆トラップ起動検証
└── docs/                          # 開発者ドキュメント
    ├── README.md                  # 本仕様・実装完了報告書 (日本語)
    ├── setup.md                   # ビルド＆セットアップガイド
    └── eos_setup.md               # EOSポータル設定手順
```

---

## 2. 完成した主要コアサブシステムの実装仕様

### 2.1. ビルド成果物の出力先設定 (Release/ Allocation)
- Windowsビルド: `Release/windows/TheStations.exe`
- macOSビルド: `Release/mac/TheStationsApp`
- Android/Linuxビルド: `Release/android/` / `Release/linux/`

### 2.2. プロシージャルアセット生成 (`graphics/` & `audio/`)
- **テクスチャ生成 (`TextureGenerator`)**: 512x512 コンクリート汚れ・錆テクスチャ、512x512 タイル血痕スプラッターテクスチャ、256x256 パーリンノイズ フィルムグレインテクスチャをメモリ上で動的生成。
- **オーディオDSP波形合成 (`AudioDSPGenerator`)**: 電車走行音（低周波ノイズ＋ドップラー周波数変調）、風のうねり音（バンドパスフィルタ付きホワイトノイズ）、館内アナウンス 4音チャイム（PCM 16bit 44.1kHz波形）をリアルタイム合成。

### 2.3. プロシージャルレベル生成 & 空間不可逆性 (`LevelGenerator`)
- **50種類の基本ステーションカタログ**: 地方の無人駅、大都市地下鉄、ターミナル駅、海外の古びた駅、工事中の坑道まで全50種類のステーションテンプレートを完全定義。
- **決定性テスト通過**: 同一シード値で完全に同一の空間ノードが再現可能。
- **空間破棄 (不可逆性)**: ドア通過後、旧ノードのメモリを自動解放。元の空間へは戻れない時空のねじれ構造を実装。

### 2.4. Epic Online Services (EOS) P2P ネットワーク (`EOSNetworkManager`)
- **衝突耐性付き5桁ルームコード発行**: 1000回以上の連続生成テストで高エントロピー・無衝突を実証。
- **CRC32 パケット検証付き P2P 通信**: 位置・状態データを構造体バイナリパッキングし、CRC32チェックサム検証同期。

### 2.5. セキュリティ & 海賊版報復トラップエンジン (`EscapeAndAntiCheatEngine`)
- **FIPS 180-2 準拠 SHA256 バイナリ検証**: 起動時に実行ファイルのバイナリを直接ディスクから読み込み、標準SHA-256ハッシュ値を計算。改ざん検知時に `m_isTrappedUser = true` をセット。
- **詰みトラップ**: 改ざんフラグ検知時、50%の確率で「案B: 即死ループ」または「案C: 無限空間」を発生。

---

## 3. ビルド・動作検証結果

```bash
mkdir build
cd build
cmake ..
make -j4
ctest --output-on-failure
```

### テスト実行結果 (CTest)
```
1/7 Test #1: test_anticheat ...................   Passed    0.01 sec
2/7 Test #2: test_biometric ...................   Passed    0.00 sec
3/7 Test #3: test_entity ......................   Passed    0.00 sec
4/7 Test #4: test_eos .........................   Passed    0.01 sec
5/7 Test #5: test_level .......................   Passed    0.00 sec
6/7 Test #6: test_sanity ......................   Passed    0.00 sec
7/7 Test #7: test_ui ..........................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 7
```

全 7 項目の厳格なテストスイートが正常に合格することを確認済みです。
