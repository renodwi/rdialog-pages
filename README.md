# rdialog-pages

[![sampctl](https://img.shields.io/badge/sampctl-rdialog--pages-2f2f2f.svg?style=for-the-badge)](https://github.com/renodwi/rdialog-pages)

Sebuah *include* sederhana untuk **SA-MP** yang memungkinkan penggunaan **dialog dengan sistem halaman (paginated dialog)**.  
Library ini membantu kamu menampilkan daftar panjang data dalam *dialog* dengan tombol **“Next”** dan **“Back”**, tanpa perlu membuat banyak dialog manual.

---

## Installation

Simply install to your project:

```bash
sampctl package install renodwi/rdialog-pages
````

Include in your code and begin using the library:

```pawn
#include <rdialog-pages>
```

---

## Usage

### 📘 Konsep Utama

Library ini meniru perilaku *paginated UI* di SA-MP dengan cara menyimpan daftar item dalam array per-player, lalu menampilkannya secara bertahap melalui sistem dialog bawaan (`ShowPlayerDialog`).

Kamu dapat:

* Menambahkan item ke daftar dialog.
* Mengatur jumlah item per halaman.
* Menambahkan tombol navigasi “Next” dan “Back”.
* Menangani respons pemain dari halaman tertentu.

---

### ⚙️ API Reference

#### **ClearPlayerDialogPages(playerid)**

Menghapus seluruh daftar item dialog pemain dan mengatur ulang halaman ke awal.

**Parameters:**

* `playerid`: ID pemain yang datanya akan dihapus.

**Returns:** `1`

---

#### **AddPlayerDialogPages(playerid, const listitem[])**

Menambahkan item baru ke daftar dialog pemain.

**Parameters:**

* `playerid`: ID pemain target.
* `listitem[]`: Teks item yang akan ditampilkan pada dialog.

**Returns:**

* `1` jika berhasil.
* `0` jika jumlah item sudah mencapai batas `MAX_RDIALOG_PAGES` (500).

---

#### **ShowPlayerDialogPages**

Fungsi utama untuk menampilkan dialog dengan sistem halaman.

**Parameters:**

```pawn
ShowPlayerDialogPages(playerid, const function[], style, const caption[], const button1[], const button2[], item_per_page = 10, const nextbutton[] = "{FF0000}Halaman Selan>>>", const backbutton[] = "{FF0000}<<< Halaman Sebelumnya");
```

**Deskripsi:**

* `function`: Nama callback yang akan dipanggil saat pemain memilih item.
* `style`: Tipe dialog (mis. `DIALOG_STYLE_LIST`, `DIALOG_STYLE_TABLIST_HEADERS`, dll).
* `caption`: Judul dialog.
* `button1` dan `button2`: Label tombol dialog.
* `item_per_page`: Jumlah item per halaman (default 10).
* `nextbutton` dan `backbutton`: Label navigasi antar halaman.

---

### 🔁 Callback Integrasi

Callback bawaan SA-MP `OnDialogResponse` di-*hook* agar sistem rDialogPages bisa bekerja otomatis.
Kamu hanya perlu mendefinisikan callback baru dengan nama sesuai argumen `function` pada `ShowPlayerDialogPages`.

**Contoh:**

```pawn
DialogPages:MyMenu(playerid, response, listitem, inputtext[])
{
    if (!response) return 1;

    switch (listitem)
    {
        case 0: SendClientMessage(playerid, -1, "Kamu memilih item 1");
        case 1: SendClientMessage(playerid, -1, "Kamu memilih item 2");
    }
    return 1;
}
```

---

### 🧩 Contoh Penggunaan Lengkap

```pawn
#include <a_samp>
#include <rdialog-pages>

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!strcmp(cmdtext, "/testpage", true))
    {
        ClearPlayerDialogPages(playerid);

        AddPlayerDialogPages(playerid, "Item 1");
        AddPlayerDialogPages(playerid, "Item 2");
        AddPlayerDialogPages(playerid, "Item 3");
        AddPlayerDialogPages(playerid, "Item 4");
        AddPlayerDialogPages(playerid, "Item 5");
        AddPlayerDialogPages(playerid, "Item 6");

        ShowPlayerDialogPages(playerid,
            "MyMenu",
            DIALOG_STYLE_LIST,
            "Contoh rDialog Pages",
            "Pilih",
            "Tutup",
            3 // Item per page
        );
        return 1;
    }
    return 0;
}

DialogPages:MyMenu(playerid, response, listitem, inputtext[])
{
    if (!response) return 1;
    new msg[64];
    format(msg, sizeof msg, "Kamu memilih item %d", listitem + 1);
    SendClientMessage(playerid, -1, msg);
    return 1;
}
```

---

## Testing

Untuk mengetes library ini, jalankan paket dengan perintah berikut:

```bash
sampctl package run
```

Saat dijalankan, gunakan perintah `/testpage` di dalam game untuk mencoba fitur **paged dialog**.

---

## Catatan

* Maksimal item per pemain: `500`
* Panjang string per item: `256`
* Fungsi otomatis menangani `\n` dan `\r` agar tampilan dialog tetap rapi.
* Kompatibel dengan sistem callback eksternal lainnya berkat alias `rdlgpgs_OnDialogResponse`.

---

© 2025 – Created by [@renodwi](https://github.com/renodwi)