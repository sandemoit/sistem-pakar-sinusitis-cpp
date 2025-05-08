#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <map>
#include <cmath>
using namespace std;

// Struktur untuk menyimpan gejala
struct Gejala
{
    string kode;
    string nama;
    double cf_user;
};

// Struktur untuk menyimpan penyakit
struct Penyakit
{
    string kode;
    string nama;
    vector<string> kode_gejala;
    map<string, double> cf_pakar;
    double cf_hasil;
    vector<pair<string, double>> cf_hitung; // Untuk menyimpan perhitungan detail CF
};

// Fungsi untuk membersihkan input buffer
void clearInputBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Fungsi untuk menghitung CF kombinasi dengan benar
double hitungCFKombinasi(double cf1, double cf2)
{
    if (cf1 >= 0 && cf2 >= 0)
    {
        // Kedua CF positif
        return cf1 + cf2 * (1 - cf1);
    }
    else if (cf1 < 0 && cf2 < 0)
    {
        // Kedua CF negatif
        return cf1 + cf2 * (1 + cf1);
    }
    else
    {
        // CF1 dan CF2 berbeda tanda
        return (cf1 + cf2) / (1 - min(fabs(cf1), fabs(cf2)));
    }
}

// Fungsi untuk menjalankan diagnosa
void runDiagnosa()
{
    string nama;
    cout << "Masukkan nama Anda: ";
    getline(cin, nama);

    // Inisialisasi daftar gejala
    vector<Gejala> gejala = {
        {"G01", "Hidung Tersumbat", 0.0},
        {"G02", "Nyeri pada wajah", 0.0},
        {"G03", "Berkurangnya daya pengecap", 0.0},
        {"G04", "Berkurangnya daya penciuman", 0.0},
        {"G05", "Lendir Ingus Keluar dari hidung dalam jumlah kecil terus menerus", 0.0},
        {"G06", "Batuk", 0.0},
        {"G07", "Demam", 0.0},
        {"G08", "Nyeri sekitar pipi", 0.0},
        {"G09", "Nyeri sekitar hidung", 0.0},
        {"G010", "Nyeri pada gigi/gusi", 0.0},
        {"G011", "Sakit Kepala", 0.0},
        {"G012", "Nyeri antara mata", 0.0},
        {"G013", "Nyeri dahi", 0.0},
        {"G014", "Nyeri belakang bola mata", 0.0},
        {"G015", "Nafas Berbau", 0.0}};

    // Inisialisasi daftar penyakit dengan nilai CF pakar
    vector<Penyakit> penyakit = {
        {"P001", "Sinusitis Frontalis", {"G01", "G02", "G03", "G04", "G07", "G011", "G013", "G014"}, {{"G01", 0.4}, {"G02", 0.6}, {"G03", 0.4}, {"G04", 0.6}, {"G07", 0.4}, {"G011", 0.4}, {"G013", 0.6}, {"G014", 0.6}}, 0.0},

        {"P002", "Sinusitis Maksilaris", {"G01", "G02", "G03", "G05", "G08", "G010", "G011", "G015"}, {{"G01", 0.4}, {"G02", 0.6}, {"G03", 0.6}, {"G05", 0.6}, {"G08", 0.6}, {"G010", 0.6}, {"G011", 0.4}, {"G015", 0.6}}, 0.0},

        {"P003", "Sinusitis Etmoidalis", {"G01", "G02", "G03", "G04", "G06", "G09", "G012"}, {{"G01", 0.6}, {"G02", 0.6}, {"G03", 0.4}, {"G04", 0.6}, {"G06", 0.6}, {"G09", 0.6}, {"G012", 0.6}}, 0.0},

        {"P004", "Sinusitis Sfenoidalis", {"G01", "G02", "G011", "G014"}, {{"G01", 0.4}, {"G02", 0.4}, {"G011", 0.6}, {"G014", 0.6}}, 0.0}};

    // Nilai CF user sesuai jawaban - 5 options
    // Menggunakan nilai CF yang sesuai dengan skala 0-100%
    vector<double> cf_nilai_user = {-1.0, -0.4, 0, 0.4, 1.0};
    vector<string> cf_term = {"Pasti Tidak", "Mungkin Tidak", "Tidak Tahu",
                              "Mungkin Iya", "Pasti Iya"};

    cout << "\n===================================================" << endl;
    cout << "SISTEM PAKAR DIAGNOSA PENYAKIT SINUSITIS MENGGUNAKAN" << endl;
    cout << "           METODE CERTAINTY FACTOR" << endl;
    cout << "===================================================" << endl;
    cout << "\nHalo " << nama << ", silahkan jawab pertanyaan berikut sesuai dengan gejala yang Anda alami:" << endl;
    cout << "Pilihan jawaban:" << endl;
    cout << "1. Pasti Tidak     4. Mungkin Iya" << endl;
    cout << "2. Mungkin Tidak   5. Pasti Iya" << endl;
    cout << "3. Tidak Tahu" << endl
         << endl;

    // Untuk menyimpan jawaban user
    map<string, int> jawaban_user;

    // Mengambil input user untuk setiap gejala
    for (int i = 0; i < gejala.size(); i++)
    {
        int pilihan;
        cout << i + 1 << ". " << gejala[i].nama << " (" << gejala[i].kode << ")" << endl;
        cout << "   Masukkan pilihan angka (1-5): ";

        while (!(cin >> pilihan) || pilihan < 1 || pilihan > 5)
        {
            clearInputBuffer();
            cout << "   Pilihan tidak valid. Masukkan pilihan (1-5): ";
        }

        // Simpan nilai CF user dan jawaban user
        gejala[i].cf_user = cf_nilai_user[pilihan - 1];
        jawaban_user[gejala[i].kode] = pilihan;
    }

    // Menghitung CF untuk setiap penyakit
    for (auto &p : penyakit)
    {
        vector<double> cf_gejala;
        p.cf_hitung.clear(); // Hapus perhitungan lama

        // Hitung CF untuk setiap gejala yang terkait dengan penyakit ini
        for (const auto &kode_gejala : p.kode_gejala)
        {
            // Temukan gejala dalam daftar
            for (const auto &g : gejala)
            {
                if (g.kode == kode_gejala)
                {
                    // Hitung CF untuk gejala ini
                    double cf = p.cf_pakar[kode_gejala] * g.cf_user;
                    // Simpan perhitungan CF untuk detail
                    p.cf_hitung.push_back({g.kode, cf});
                    if (cf != 0)
                    { // Abaikan CF = 0 (Tidak Tahu)
                        cf_gejala.push_back(cf);
                    }
                    break;
                }
            }
        }

        // Kombinasikan semua CF gejala untuk penyakit ini
        if (!cf_gejala.empty())
        {
            double cf_combined = cf_gejala[0];
            for (size_t i = 1; i < cf_gejala.size(); i++)
            {
                cf_combined = hitungCFKombinasi(cf_combined, cf_gejala[i]);
            }
            p.cf_hasil = cf_combined;
        }
    }

    // Mengurutkan hasil diagnosa berdasarkan nilai CF tertinggi (absolut)
    sort(penyakit.begin(), penyakit.end(), [](const Penyakit &a, const Penyakit &b)
         { return fabs(a.cf_hasil) > fabs(b.cf_hasil); });

    // Menampilkan hasil diagnosa
    cout << "\n===================================================" << endl;
    cout << "                   HASIL DIAGNOSA" << endl;
    cout << "===================================================" << endl;
    cout << "Nama: " << nama << endl
         << endl;

    // Tabel hasil input user
    cout << "GEJALA YANG DIALAMI:" << endl;
    cout << "+------+----------------+-------------------------+-------+-------+------+" << endl;
    cout << "| No.  | Kode Gejala    | Gejala                  | CF    | Nilai | CF   |" << endl;
    cout << "|      |                |                         | Pakar | User  |      |" << endl;
    cout << "+------+----------------+-------------------------+-------+-------+------+" << endl;

    int no = 1;
    // Vector untuk menyimpan semua gejala yang akan ditampilkan (untuk filter "Tidak Tahu")
    vector<pair<Gejala, int>> gejala_display;

    for (const auto &g : gejala)
    {
        if (jawaban_user[g.kode] != 3)
        { // Tampilkan hanya yang tidak "Tidak Tahu"
            gejala_display.push_back({g, jawaban_user[g.kode]});
        }
    }

    // Tampilkan gejala berdasarkan urutan kode
    for (const auto &item : gejala_display)
    {
        const Gejala &g = item.first;
        int pilihan = item.second;

        cout << "| " << setw(4) << left << no++ << " | ";
        cout << setw(15) << left << g.kode << " | ";
        cout << setw(25) << left << g.nama << " | ";

        // Cari CF pakar untuk gejala ini
        double cf_pakar = 0.0;
        for (const auto &p : penyakit)
        {
            if (p.cf_pakar.find(g.kode) != p.cf_pakar.end())
            {
                cf_pakar = p.cf_pakar.at(g.kode);
                break;
            }
        }

        cout << setw(5) << right << fixed << setprecision(1) << cf_pakar << " | ";
        cout << setw(5) << right << cf_nilai_user[pilihan - 1] << " | ";
        cout << setw(4) << right << fixed << setprecision(2) << (cf_pakar * g.cf_user) << " |" << endl;
    }
    cout << "+------+----------------+-------------------------+-------+-------+------+" << endl;

    // Menampilkan hasil perhitungan untuk setiap penyakit
    cout << "\nHASIL PERHITUNGAN:" << endl;
    for (const auto &p : penyakit)
    {
        cout << "\n"
             << p.nama << " (" << p.kode << ")" << endl;

        // Detail perhitungan CF (jika ada)
        if (!p.cf_hitung.empty())
        {
            // Filter hanya CF yang tidak nol
            vector<pair<string, double>> non_zero_cf;
            for (const auto &cf_pair : p.cf_hitung)
            {
                if (cf_pair.second != 0)
                {
                    non_zero_cf.push_back(cf_pair);
                }
            }

            if (non_zero_cf.empty())
            {
                cout << "Tidak ada gejala signifikan." << endl;
                continue;
            }

            double cf_old = non_zero_cf[0].second;
            cout << "CF1 = " << fixed << setprecision(4) << cf_old << endl;

            for (size_t i = 1; i < non_zero_cf.size(); i++)
            {
                double cf_current = non_zero_cf[i].second;
                double cf_combined = hitungCFKombinasi(cf_old, cf_current);

                cout << "CF" << (i + 1) << " = " << fixed << setprecision(4) << cf_old << " + "
                     << fixed << setprecision(4) << cf_current << " * (1 ";

                if (cf_old >= 0 && cf_current >= 0)
                {
                    cout << "- " << fixed << setprecision(4) << cf_old;
                }
                else if (cf_old < 0 && cf_current < 0)
                {
                    cout << "+ " << fixed << setprecision(4) << cf_old;
                }
                else
                {
                    cout << "/ (1 - min(" << fabs(cf_old) << ", " << fabs(cf_current) << "))";
                }

                cout << ") = " << fixed << setprecision(4) << cf_combined << endl;

                cf_old = cf_combined;
            }

            // Konversi CF hasil ke persentase kemungkinan penyakit
            // Jika CF negatif, konversi menjadi persentase rendah (<50%)
            // Jika CF positif, konversi menjadi persentase tinggi (>50%)
            double persentase;
            if (p.cf_hasil < 0)
            {
                // Konversi CF negatif (-1 sampai 0) ke persentase (0% sampai 50%)
                persentase = 50 * (1 + p.cf_hasil);
            }
            else
            {
                // Konversi CF positif (0 sampai 1) ke persentase (50% sampai 100%)
                persentase = 50 + (50 * p.cf_hasil);
            }

            cout << "Persentase yang diperoleh dari " << p.nama << ", yaitu:" << endl;

            // Tampilkan perhitungan konversi
            if (p.cf_hasil < 0)
            {
                cout << "CF = " << fixed << setprecision(4) << p.cf_hasil << " (negatif)" << endl;
                cout << "Persentase = 50% * (1 + " << fixed << setprecision(4) << p.cf_hasil << ") = "
                     << fixed << setprecision(2) << persentase << "%" << endl;
            }
            else
            {
                cout << "CF = " << fixed << setprecision(4) << p.cf_hasil << " (positif)" << endl;
                cout << "Persentase = 50% + (50% * " << fixed << setprecision(4) << p.cf_hasil << ") = "
                     << fixed << setprecision(2) << persentase << "%" << endl;
            }
        }
        else
        {
            cout << "Tidak ada gejala yang signifikan." << endl;
        }
    }

    // Menampilkan tabel hasil dengan persentase baru
    cout << "\nRINGKASAN HASIL DIAGNOSA:" << endl;
    cout << "+------------------------+--------+------------------+" << endl;
    cout << "| Penyakit               | Kode   | Persentase       |" << endl;
    cout << "+------------------------+--------+------------------+" << endl;

    for (const auto &p : penyakit)
    {
        cout << "| " << setw(22) << left << p.nama << " | ";
        cout << setw(6) << left << p.kode << " | ";

        // Konversi persentase sesuai skala baru
        double persentase;
        if (p.cf_hasil < 0)
        {
            // Konversi CF negatif (-1 sampai 0) ke persentase (0% sampai 50%)
            persentase = 50 * (1 + p.cf_hasil);
        }
        else
        {
            // Konversi CF positif (0 sampai 1) ke persentase (50% sampai 100%)
            persentase = 50 + (50 * p.cf_hasil);
        }

        cout << setw(16) << right << fixed << setprecision(2) << persentase << "% |" << endl;
    }
    cout << "+------------------------+--------+------------------+" << endl;

    // Cek apakah ada diagnosis positif dan menampilkan kesimpulan
    if (penyakit[0].cf_hasil >= 0 && penyakit[0].cf_hasil <= 1)
    {
        double persentase = 50 + (50 * penyakit[0].cf_hasil);

        if (persentase >= 70)
        {
            cout << "\nKESIMPULAN: Berdasarkan gejala yang Anda alami, kemungkinan besar Anda menderita "
                 << penyakit[0].nama << " dengan tingkat kepercayaan " << fixed << setprecision(2)
                 << persentase << "%" << endl;
        }
        else if (persentase >= 50)
        {
            cout << "\nKESIMPULAN: Berdasarkan gejala yang Anda alami, Anda mungkin menderita "
                 << penyakit[0].nama << " dengan tingkat kepercayaan " << fixed << setprecision(2)
                 << persentase << "%" << endl;
        }
    }
    else if (penyakit[0].cf_hasil >= -1 && penyakit[0].cf_hasil < 0)
    {
        double persentase = 50 * (1 + penyakit[0].cf_hasil);

        if (persentase <= 30)
        {
            cout << "\nKESIMPULAN: Berdasarkan gejala yang Anda alami, kemungkinan besar Anda tidak menderita "
                 << "penyakit sinusitis dengan tingkat kepercayaan " << fixed << setprecision(2)
                 << (100 - persentase) << "%" << endl;
        }
        else if (persentase < 50)
        {
            cout << "\nKESIMPULAN: Berdasarkan gejala yang Anda alami, Anda mungkin tidak menderita "
                 << "penyakit sinusitis dengan tingkat kepercayaan " << fixed << setprecision(2)
                 << (100 - persentase) << "%" << endl;
        }
    }
    else
    {
        cout << "\nKESIMPULAN: Tidak ada kesimpulan yang dapat ditarik dari gejala yang Anda alami." << endl;
    }
}

int main()
{
    char lanjut = 'Y';

    while (toupper(lanjut) == 'Y')
    {
// Bersihkan layar
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        // Jalankan diagnosa
        runDiagnosa();

        // Tanya apakah ingin mengulang
        cout << "\nApakah Anda ingin melakukan diagnosa lagi? (Y/N): ";
        cin >> lanjut;
        clearInputBuffer();
    }

    cout << "\nTerima kasih telah menggunakan Sistem Pakar Diagnosa Penyakit Sinusitis!" << endl;

    return 0;
}
