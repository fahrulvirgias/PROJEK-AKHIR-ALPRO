#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Untuk menggunakan tipe data bool (true/false)

// Enum untuk status mobil yang lebih mudah dibaca
typedef enum {
    DISEWA,    // Akan bernilai 0
    TERSEDIA   // Akan bernilai 1
} StatusMobil;

#define MAX_MOBIL 10
#define NAMA_FILE_DATABASE "database_mobil.txt" 

typedef struct {
    int id;
    char namaMobil[50];
    char jenis[30];
    long hargaPerHari;
    StatusMobil status; 
} Mobil;

// Struct utama yang mengelola seluruh sistem rental
typedef struct {
    Mobil daftarMobil[MAX_MOBIL];
    int jumlahMobil;
} RentalSystem;


void sistem_inisialisasi(RentalSystem *sys);
void sistem_simpanKeFile(const RentalSystem *sys);
bool sistem_muatDariFile(RentalSystem *sys);
int sistem_cariMobilByID(const RentalSystem *sys, int id); l

void tampilkanHeader();
void menuLihatMobil(const RentalSystem *sys);
void menuSewaMobil(RentalSystem *sys);
void menuPengembalianMobil(RentalSystem *sys);
void clearScreen();
void bersihkanInputBuffer();


int main() {
    int pilihan;
    RentalSystem rental; 
    sistem_inisialisasi(&rental); 

    do {
        clearScreen();
        tampilkanHeader();
        printf("\n=== MENU UTAMA ===\n");
        printf("1. Lihat Mobil (Daftar & Harga)\n");
        printf("2. Sewa Mobil (Hitung Biaya)\n");
        printf("3. Pengembalian Mobil\n");
        printf("0. Keluar\n");
        printf("Pilihan Anda: ");
        if (scanf("%d", &pilihan) != 1) pilihan = -1; 
        bersihkanInputBuffer(); 

        switch (pilihan) {
            case 1:
                menuLihatMobil(&rental);
                break;
            case 2:
                menuSewaMobil(&rental);
                break;
            case 3:
                menuPengembalianMobil(&rental);
                break;
            case 0:
                // Simpan semua perubahan terakhir ke file sebelum program benar-benar keluar.
                sistem_simpanKeFile(&rental);
                printf("\nTerima kasih telah menggunakan aplikasi ini.\n");
                break;
            default:
                printf("\nPilihan tidak valid.\n");
                break;
        }
        
        if (pilihan != 0) {
            printf("\nTekan Enter untuk kembali ke menu...");
            bersihkanInputBuffer();
        }

    } while (pilihan != 0);

    return 0;
}

void sistem_inisialisasi(RentalSystem *sys) {
    // Jika file tidak berhasil dimuat (misal: file tidak ada), buat data awal.
    if (!sistem_muatDariFile(sys)) {
        printf("Database tidak ditemukan. Membuat data awal...\n");
        
        sys->jumlahMobil = 0; // Pastikan jumlah mobil adalah 0 sebelum inisialisasi

        // Data Mobil Awal
        sys->daftarMobil[0] = (Mobil){101, "Toyota Supra", "Sport", 300000, TERSEDIA};
        sys->daftarMobil[1] = (Mobil){102, "BMW M4", "Coupe", 250000, TERSEDIA};
        sys->daftarMobil[2] = (Mobil){103, "Ferrari SF90", "Supercar", 800000, TERSEDIA};
        sys->daftarMobil[3] = (Mobil){104, "McLaren 720s", "Supercar", 1500000, TERSEDIA};
        sys->daftarMobil[4] = (Mobil){105, "Porsche 911", "Sport", 1500000, TERSEDIA};
        sys->daftarMobil[5] = (Mobil){106, "Lamborghini Revuelto", "Supercar", 1500000, TERSEDIA};
        sys->jumlahMobil = 6;

        sistem_simpanKeFile(sys);
        printf("Database baru berhasil dibuat: %s\n", NAMA_FILE_DATABASE);
        printf("Tekan Enter untuk melanjutkan...");
        bersihkanInputBuffer();
    }
}

void sistem_simpanKeFile(const RentalSystem *sys) {
    FILE *file = fopen(NAMA_FILE_DATABASE, "w");
    if (file == NULL) {
        printf("Gagal membuka file database untuk ditulis.\n");
        return;
    }

    for (int i = 0; i < sys->jumlahMobil; i++) {
        fprintf(file, "%d|%s|%s|%ld|%d\n",
                sys->daftarMobil[i].id,
                sys->daftarMobil[i].namaMobil,
                sys->daftarMobil[i].jenis,
                sys->daftarMobil[i].hargaPerHari,
                sys->daftarMobil[i].status);
    }

    fclose(file);
}

bool sistem_muatDariFile(RentalSystem *sys) {
    FILE *file = fopen(NAMA_FILE_DATABASE, "r");
    if (file == NULL) {
        return false; 
    }

    sys->jumlahMobil = 0;
    char buffer[256]; 
    
    int statusFromFile;
    
    while (sys->jumlahMobil < MAX_MOBIL && fgets(buffer, sizeof(buffer), file) != NULL) {
        if (sscanf(buffer, "%d|%49[^|]|%29[^|]|%ld|%d",
                   &sys->daftarMobil[sys->jumlahMobil].id,
                   sys->daftarMobil[sys->jumlahMobil].namaMobil,
                   sys->daftarMobil[sys->jumlahMobil].jenis,
                   &sys->daftarMobil[sys->jumlahMobil].hargaPerHari,
                   &statusFromFile) == 5) {
            sys->daftarMobil[sys->jumlahMobil].status = (StatusMobil)statusFromFile;
            sys->jumlahMobil++;
        }
    }

    fclose(file);
    return true; 
}

int sistem_cariMobilByID(const RentalSystem *sys, int id) {
    for (int i = 0; i < sys->jumlahMobil; i++) {
        if (sys->daftarMobil[i].id == id) {
            return i; 
        }
    }
    return -1;
}

void tampilkanHeader() {
    printf("==========================================\n");
    printf("      APLIKASI RENTAL MOBIL KELOMPOK 10\n");
    printf("==========================================\n");
}

void menuLihatMobil(const RentalSystem *sys) {
    clearScreen();
    printf("\n=== DAFTAR MOBIL TERSEDIA ===\n");
    printf("%-5s %-20s %-10s %-15s %-10s\n", "ID", "Nama Mobil", "Jenis", "Harga/Hari", "Status");
    printf("----------------------------------------------------------------\n");

    if (sys->jumlahMobil == 0) {
        printf("Tidak ada mobil dalam sistem.\n");
        return;
    }

    for (int i = 0; i < sys->jumlahMobil; i++) {
        
        const char* statusStr = (sys->daftarMobil[i].status == TERSEDIA) ? "Tersedia" : "Disewa";

        printf("%-5d %-20s %-10s Rp.%-13ld %-10s\n", 
            sys->daftarMobil[i].id, 
            sys->daftarMobil[i].namaMobil, 
            sys->daftarMobil[i].jenis, 
            sys->daftarMobil[i].hargaPerHari, 
            statusStr);
    }
}

void menuSewaMobil(RentalSystem *sys) {
    int idTarget, durasi;
    long totalBiaya;

    menuLihatMobil(sys); 
    printf("\n--- FORM SEWA MOBIL ---\n");
    printf("Masukkan ID Mobil yang ingin disewa: ");
    if (scanf("%d", &idTarget) != 1) {
        bersihkanInputBuffer();
        printf("\n[!] Input ID tidak valid.\n");
        return;
    }
    bersihkanInputBuffer();

    int indexMobil = sistem_cariMobilByID(sys, idTarget);

    if (indexMobil == -1) {
        printf("\n[!] ID Mobil tidak ditemukan.\n");
        return;
    }

    if (sys->daftarMobil[indexMobil].status == DISEWA) {
        printf("\n[!] Maaf, mobil %s sedang tidak tersedia.\n", sys->daftarMobil[indexMobil].namaMobil);
    } else {
        printf("Mobil dipilih: %s (%s)\n", sys->daftarMobil[indexMobil].namaMobil, sys->daftarMobil[indexMobil].jenis);
        printf("Harga Sewa: Rp.%ld / hari\n", sys->daftarMobil[indexMobil].hargaPerHari);
        
        printf("Masukkan lama sewa (hari): ");
        if (scanf("%d", &durasi) != 1 || durasi <= 0) {
            bersihkanInputBuffer();
            printf("\n[Batal] Durasi sewa tidak valid.\n");
            return;
        }
        bersihkanInputBuffer();

        totalBiaya = sys->daftarMobil[indexMobil].hargaPerHari * durasi;

        printf("\n--------------------------\n");
        printf("Total Biaya: Rp.%ld\n", totalBiaya);
        printf("--------------------------\n");
        
        char konfirmasi;
        printf("Konfirmasi sewa? (y/n): ");
        scanf(" %c", &konfirmasi);
        bersihkanInputBuffer();

        if (konfirmasi == 'y' || konfirmasi == 'Y') {
            sys->daftarMobil[indexMobil].status = DISEWA;
            sistem_simpanKeFile(sys);
            printf("\n[Sukses] Mobil berhasil disewa!\n");
        } else {
            printf("\n[Batal] Transaksi dibatalkan.\n");
        }
    }
}

void menuPengembalianMobil(RentalSystem *sys) { 
    int idTarget;

    clearScreen();
    printf("\n=== PENGEMBALIAN MOBIL ===\n");
    printf("Masukkan ID Mobil yang dikembalikan: ");
    if (scanf("%d", &idTarget) != 1) {
        bersihkanInputBuffer();
        printf("\n[!] Input ID tidak valid.\n");
        return;
    }
    bersihkanInputBuffer();

    int indexMobil = sistem_cariMobilByID(sys, idTarget);

    if (indexMobil == -1) {
        printf("\n[!] ID Mobil tidak ditemukan.\n");
        return;
    }

    if (sys->daftarMobil[indexMobil].status == TERSEDIA) {
        printf("\n[!] Mobil ini memang statusnya sudah tersedia (belum disewa).\n");
    } else {
        char konfirmasi;
        printf("Mobil: %s\n", sys->daftarMobil[indexMobil].namaMobil);
        printf("Kembalikan mobil ini? (y/n): ");
        scanf(" %c", &konfirmasi);
        bersihkanInputBuffer();

        if (konfirmasi == 'y' || konfirmasi == 'Y') {
            sys->daftarMobil[indexMobil].status = TERSEDIA;
            sistem_simpanKeFile(sys); 
            printf("\n[Sukses] Mobil telah dikembalikan dan status menjadi Tersedia.\n");
        } else {
            printf("\n[Batal] Proses dibatalkan.\n");
        }
    }
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void bersihkanInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}