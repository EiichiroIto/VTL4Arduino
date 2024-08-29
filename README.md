# Very Tiny Language for Arduino
これは [Very Tiny Language](https://manx-docs.org/mirror/harte/Altair/MITS_Altair_680_Very_Tiny_Language_VTL-2_Manual.PDF) の Arduino 移植版です。

[C言語で書かれた実装](http://middleriver.chagasi.com/electronics/vtl.html)を、[Smalltalk に移植した](https://github.com/EiichiroIto/VeryTinyLanguage/blob/main/README.ja.md)後で、再度C言語で書き直したものです。

これは Game 言語を Arduino 上に実装するための準備プロジェクトで、この VTL を拡張する形で Game 言語を実装していきます。
そのため、この VTL で Arduino 固有の機能を利用することは想定していません。

シリアルポート経由で接続すると、VTL for Arduino というプロンプトが表示され、試すことができます。

```
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [======    ]  60.3% (used 1234 bytes from 2048 bytes)
Flash: [=         ]  12.3% (used 3958 bytes from 32256 bytes)
```

メモリエリアには 1024 バイト確保していますが、システムで264バイト使用するため、プログラムでは760バイトほど使うことができます。
