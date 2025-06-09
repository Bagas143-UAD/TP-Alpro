#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

const int MAX_ITEMS = 100;

struct fitur{
	int pilih;
	string admin; 
	string nama[3]={"Ian", "Bagas", "Zeicha"};
	double pass;
	double pin[3]={2400018124, 2400018143, 2400018148};
};

struct Item {
    int id;
    string nama;
    double harga;
    int stock;
};

Item items[MAX_ITEMS];
int itemCount = 0;

void input() {
    if (itemCount >= MAX_ITEMS) {
        cout << "\n Gagal menambahkan barang, kapasitas penuh!\n";
        return;
    }

    cout << "\n Tambah Barang Baru\n";
    items[itemCount].id = itemCount + 1;
    cout << "Nama barang  : ";
    cin.ignore();
    getline(cin, items[itemCount].nama);
    cout << "Harga barang : ";
    cin >> items[itemCount].harga;
    cout << "Stok barang  : ";
    cin >> items[itemCount].stock;

    itemCount++;
    cout << "? Barang berhasil ditambahkan!\n";
}

void tampilan() {
    if (itemCount == 0) {
        cout << "\n Tidak ada barang yang tersedia.\n";
        return;
    }

    cout << "\n Daftar Barang\n";
    cout << left << setw(5) << "ID" << setw(20) << "Nama" << setw(10) << "Harga" << setw(10) << "Stok" << endl;
    for (int i = 0; i < itemCount; i++) {
        cout << left << setw(5) << items[i].id
             << setw(20) << items[i].nama
             << setw(10) << items[i].harga
             << setw(10) << items[i].stock << endl;
    }
}

void simpan() {
    ofstream file("items.txt");
    if (!file) {
        cout << "\n Gagal membuka file untuk menyimpan data.\n";
        return;
    }

    for (int i = 0; i < itemCount; i++) {
        file << items[i].id << "," << items[i].nama << "," << items[i].harga << "," << items[i].stock << endl;
    }

    file.close();
    cout << "\n Data barang berhasil disimpan ke file!\n";
}

void tutup() {
    ifstream file("items.txt");
    if (!file) {
        cout << "\n File data tidak ditemukan, mulai dengan data kosong.\n";
        return;
    }

    itemCount = 0;
    string line;
    while (getline(file, line) && itemCount < MAX_ITEMS) {
        stringstream ss(line);
        string idStr, nama, hargaStr, stockStr;

        getline(ss, idStr, ',');
        getline(ss, nama, ',');
        getline(ss, hargaStr, ',');
        getline(ss, stockStr, ',');

        stringstream(idStr) >> items[itemCount].id;
        items[itemCount].nama = nama;
        stringstream(hargaStr) >> items[itemCount].harga;
        stringstream(stockStr) >> items[itemCount].stock;

        itemCount++;
    }

    file.close();
    cout << "\n Data barang berhasil dimuat dari file!\n";
}
double hitungDiskon(double total) {
    if (total >= 100000) return total * 0.10; // 10% diskon
    else if (total >= 50000) return total * 0.05; // 5% diskon
    else return 0;
}

void jual() {
    if (itemCount == 0) {
        cout << "\n Tidak ada barang yang tersedia untuk dijual.\n";
        return;
    }

    int id, quantity;
    tampilan();
    cout << "\n Masukkan ID barang yang ingin dibeli: ";
    cin >> id;
    cout << "Jumlah yang ingin dibeli: ";
    cin >> quantity;

    if (id < 1 || id > itemCount) {
        cout << "\n ID barang tidak valid.\n";
        return;
    }

    if (items[id - 1].stock < quantity) {
        cout << "\n Stok barang tidak mencukupi.\n";
        return;
    }

   items[id - 1].stock -= quantity;
double total = quantity * items[id - 1].harga;
double diskon = hitungDiskon(total);
double bayar = total - diskon;

cout << "\n Transaksi berhasil!\n";
cout << "Total harga    : Rp" << fixed << setprecision(2) << total << endl;
cout << "Diskon         : Rp" << diskon << endl;
cout << "Total dibayar  : Rp" << bayar << endl;

}

void laporan_stok_rendah() {
    cout << "\n Barang dengan stok rendah (<= 5):\n";
    bool ada = false;
    for (int i = 0; i < itemCount; i++) {
        if (items[i].stock <= 5) {
            cout << left << setw(5) << items[i].id
                 << setw(20) << items[i].nama
                 << setw(10) << items[i].harga
                 << setw(10) << items[i].stock << endl;
            ada = true;
        }
    }
    if (!ada) cout << "Semua barang dalam stok aman.\n";
}

void laporan_harga_tertinggi() {
    if (itemCount == 0) {
        cout << "Tidak ada barang.\n";
        return;
    }
    Item* max = &items[0];
    for (int i = 1; i < itemCount; i++) {
        if (items[i].harga > max->harga)
            max = &items[i];
    }
    cout << "\n Barang dengan harga tertinggi:\n";
    cout << "ID: " << max->id << ", Nama: " << max->nama << ", Harga: Rp" << max->harga << ", Stok: " << max->stock << endl;
}

void laporan_total_inventaris() {
    double total = 0;
    for (int i = 0; i < itemCount; i++) {
        total += items[i].harga * items[i].stock;
    }
    cout << "\n Total nilai inventaris: Rp" << total << endl;
}

void laporanMenu() {
    fitur menu;
    do {
        cout << "\n Menu Laporan:\n";
        cout << "1. Barang Stok Rendah\n";
        cout << "2. Harga Tertinggi\n";
        cout << "3. Total Inventaris\n";
        cout << "4. Kembali\n";
        cout << "Pilihan: ";
        cin >> menu.pilih;

        switch (menu.pilih) {
            case 1: 
				laporan_stok_rendah(); 
				break;
            case 2: 
				laporan_harga_tertinggi(); 
				break;
            case 3: 
				laporan_total_inventaris(); 
				break;
            case 4: 
				return;
            default: 
				cout << "Pilihan tidak valid.\n";
        }
    } while (true);
}

void resetData() {
    char confirm;
    cout << "Yakin ingin menghapus semua data? (y/n): ";
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        itemCount = 0;
        cout << " Semua data dihapus.\n";
    } else {
        cout << " Aksi dibatalkan.\n";
    }
}

void ubahHarga() {
    int id;
    double hargaBaru;
    tampilan();
    cout << "Masukkan ID barang: ";
    cin >> id;
    if (id < 1 || id > itemCount) {
        cout << "ID tidak valid.\n";
        return;
    }
    cout << "Harga baru: ";
    cin >> hargaBaru;
    items[id - 1].harga = hargaBaru;
    cout << "Harga berhasil diubah.\n";
}

void ubahStok() {
    int id, stokBaru;
    tampilan();
    cout << "Masukkan ID barang: ";
    cin >> id;
    if (id < 1 || id > itemCount) {
        cout << "ID tidak valid.\n";
        return;
    }
    cout << "Stok baru: ";
    cin >> stokBaru;
    items[id - 1].stock = stokBaru;
    cout << "Stok berhasil diubah.\n";
}

void pengaturanMenu() {
    fitur menu;
    do {
        cout << "\n Menu Pengaturan:\n";
        cout << "1. Reset Semua Data\n";
        cout << "2. Ubah Harga Barang\n";
        cout << "3. Ubah Stok Barang\n";
        cout << "4. Kembali\n";
        cout << "Pilihan: ";
        cin >> menu.pilih;

        switch (menu.pilih) {
            case 1: 
				resetData(); 
				break;
            case 2: 
				ubahHarga(); 
				break;
            case 3: 
				ubahStok(); 
				break;
            case 4: 
				return;
            	default: cout << "Pilihan tidak valid.\n";
        }
    } while (true);
}

void bantuanMenu() {
    cout << "\n Bantuan Pengguna:\n";
    cout << "- Gunakan angka untuk memilih menu.\n";
    cout << "- Semua data akan disimpan secara otomatis saat keluar.\n";
    cout << "- Simpan file 'items.txt' agar data tidak hilang.\n";
}

void tampilanadmin(){
	fitur menu;
    do {
        cout << "\n=== Sistem Manajemen Toko Swalayan ===??\n";
        cout << "1. Tambah Barang\n";
        cout << "2. Lihat Barang\n";
        cout << "3. Jual Barang\n";
        cout << "4. Simpan & Keluar\n";
        cout << "5. Laporan\n";
        cout << "6. Pengaturan\n";
        cout << "7. Bantuan\n";
        cout << "Pilih menu: ";
        cin >> menu.pilih;

        switch (menu.pilih) {
            case 1: 
				input(); 
				break;
            case 2: 
				tampilan(); 
				break;
            case 3: 
				jual(); 
				break;
            case 4: 
				simpan(); 
				cout << " Terima kasih telah menggunakan aplikasi.\n"; 
				break;
            case 5: 
				laporanMenu(); 
				break;
            case 6: 
				pengaturanMenu(); 
				break;
            case 7: 
				bantuanMenu(); 
				break;
            default: 
				cout << " Pilihan tidak valid.\n";
        }
    } while (menu.pilih != 4);
    
}



void beliBarang() {
    if (itemCount == 0) {
        cout << "\n Tidak ada barang yang tersedia untuk dibeli.\n";
        return;
    }
    

    int id, quantity;
    tampilan(); 
    cout << "\n Masukkan ID barang yang ingin dibeli: ";
    cin >> id;
    cout << "Jumlah yang ingin dibeli: ";
    cin >> quantity;

    if (id < 1 || id > itemCount) {
        cout << "\n ID barang tidak valid.\n";
        return;
    }

    if (items[id - 1].stock < quantity) {
        cout << "\n Maaf, stok tidak mencukupi.\n";
        return;
    }

    items[id - 1].stock -= quantity;
double total = items[id - 1].harga * quantity;
double diskon = hitungDiskon(total);
double bayar = total - diskon;

cout << "\n >>> Pembelian berhasil!\n";
cout << "Total harga    : Rp" << fixed << setprecision(2) << total << endl;
cout << "Diskon         : Rp" << diskon << endl;
cout << "Total dibayar  : Rp" << bayar << endl;

}

void cariProduk() {
    string keyword;
    cin.ignore(); 
    cout << ">>> Masukkan kata kunci nama produk: ";
    getline(cin, keyword);

    bool ditemukan = false;
    cout << "\nHasil Pencarian:\n";
    cout << left << setw(5) << "ID" 
         << setw(20) << "Nama" 
         << setw(10) << "Harga" 
         << setw(10) << "Stok" << endl;
    
    for (int i = 0; i < itemCount; i++) {
        
        if (items[i].nama.find(keyword) != string::npos) {
            cout << left << setw(5) << items[i].id
                 << setw(20) << items[i].nama
                 << setw(10) << items[i].harga
                 << setw(10) << items[i].stock << endl;
            ditemukan = true;
        }
    }

    if (!ditemukan) {
        cout << ">>> Produk tidak ditemukan.\n";
    }
}
void tampilanUser() {
    fitur menu;
    do {
        cout << "\n=== Menu User ===\n";
        cout << "1. Lihat Barang\n";
        cout << "2. Cari Barang\n";
        cout << "3. Beli Barang\n";
        cout << "4. Kembali\n";
        cout << "Pilih menu: ";
        cin >> menu.pilih;

        switch (menu.pilih) {
            case 1:
                tampilan(); 
                break;
            case 2:
                cariProduk(); 
                break;
            case 3:
                beliBarang(); 
                break;
            case 4:
                return;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (true);
}





void garis(){
	for(int i=0; i<=20; i++){
		cout<<"=";
	}
	cout<<endl;
}

void password(){
	fitur sandi;
	
	cout<<endl;
	
	cout<<"======= LOGIN =======";
	cout<<endl;
	cout<<"Nama Pengguna : "; cin>>sandi.admin;
	cout<<"Pin      	 : "; cin>>sandi.pass;
	
	if(sandi.admin==sandi.nama[0] && sandi.pass==sandi.pin[0]){
		system("cls");
		cout<<"Selamat datang "<<sandi.nama[0]<<endl;
		tampilanadmin();
	}
	else if(sandi.admin==sandi.nama[1] && sandi.pass==sandi.pin[1]){
		system("cls");
		cout<<"Selamat datang "<<sandi.nama[1]<<endl;
		tampilanadmin();
	}
	else if(sandi.admin==sandi.nama[2] && sandi.pass==sandi.pin[2]){
		system("cls");
		cout<<"Selamat datang "<<sandi.nama[2]<<endl;
		tampilanadmin();
	}
	else {
		system("cls");
		cout<<"Nama atau Pin anda salah!";
		cout<<endl;
		password();
	}
}

int main() {
    tutup();
    fitur menu;
    
    do {
        garis();
        cout<<"SISTEM MANAJEMEN TOKO SWALAYAN\n";
        garis();
        cout<<"1. Admin\n";
        cout<<"2. User\n";
        cout<<"3. Keluar\n";
        cout<<"Pilih Sistem: ";
        cin>>menu.pilih;
        cout<<endl;

        switch (menu.pilih) {
            case 1: 
            	system("cls");
				password(); 
				break;
           case 2:
   				 system("cls");
   				 tampilanUser(); 
    			break;

            case 3: 
				cout << ">>> Terima kasih telah menggunakan sistem.";
				cout<<endl; 
				break;
            default: 
				cout << ">>> Pilihan tidak valid!";
				cout<<endl; 
				break;
        }
    } while (menu.pilih != 3);

    return 0;
}
