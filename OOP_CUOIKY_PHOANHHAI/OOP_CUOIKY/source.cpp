#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <conio.h> 
#include <windows.h> 

using namespace std;

// ===========================================================================
// PHẦN 1: KHAI BÁO CLASS VÀ PROTOTYPE (Khai báo nguyên mẫu hàm)
// ===========================================================================

// --- KHAI BÁO GUI HELPERS ---
void TextColor(int color);
void GotoXY(int x, int y);

// --- KHAI BÁO CLASS DONHANG ---
class DonHang {
protected:
    string MaDonHang;
    string NgayDatHang;
    int GoiVanChuyen;
    double GiaTriDonHang;

public:
    DonHang(string ma, string ngay, int goi, double gia);
    DonHang();
    virtual ~DonHang();
    void setMa(string ma);
    void setNgay(string ngay);
    void setGoi(int goi);
    void setGia(double gia);
    string getMa();
    string getNgay();
    int getGoi();
    double getGia();
    virtual void InThongTin(ostream& os);
    virtual double TinhTongTien();
    virtual int getType() = 0;
};

// --- KHAI BÁO CLASS DONHANGTRONGNUOC ---
class DonHangTrongNuoc : public DonHang {
    double QuangDuong;

public:
    DonHangTrongNuoc();
    DonHangTrongNuoc(string ma, string ngay, int goi, double gia, double km);
    void InThongTin(ostream& os) override;
    double TinhTongTien() override;
    int getType() override;
    double getQuangDuong();
	void setQuangDuong(double km);
    friend istream& operator>>(istream& i, DonHangTrongNuoc& d);
    friend ostream& operator<<(ostream& o, DonHangTrongNuoc& d);
};

// --- KHAI BÁO CLASS DONHANGQUOCTE ---
class DonHangQuocTe : public DonHang {
    double ThueHaiQuan;
    double PhiHaiQuan;
    int NgayLuuKho;

public:
    DonHangQuocTe();
    DonHangQuocTe(string ma, string ngay, int goi, double gia, double thue, double phi, int nlk);

    void InThongTin(ostream& os) override;
    double TinhTongTien() override;
    int getType() override;
    double getThue();
    double getPhi();
    int getNgay();
	void setThue(double thue);
	void setPhi(double phi);
	void setNgay(int nlk);
    // Friend functions
    friend istream& operator>>(istream& i, DonHangQuocTe& d);
    friend ostream& operator<<(ostream& o, DonHangQuocTe& d);
};

// --- BIẾN TOÀN CỤC VÀ PROTOTYPE CÁC HÀM CHỨC NĂNG ---
extern vector<DonHang*> dsDonHang; // Khai báo extern (thực tế định nghĩa ở dưới)

void DocFile();
void GhiFile();
void TieuDeTrongNuoc();
void TieuDeQuocTe();
void HienThiDonHang(int filter = 0);
void ThemDonHang();
void XoaDonHang();
void TinhLoiNhuan();
int MenuDieuKhien(vector<string> menuItems);

// Định nghĩa biến toàn cục vector
vector<DonHang*> dsDonHang;

// ===========================================================================
// PHẦN 2: HÀM MAIN
// ===========================================================================

int main() {
    DocFile();

    vector<string> mainMenus = {
        "1. Them don hang moi",
        "2. Xoa don hang",
        "3. Hien thi danh sach don hang",
        "4. Tinh tong loi nhuan",
        "5. Thoat chuong trinh"
    };

    vector<string> subMenuHienThi = {
        "Hien thi TAT CA",
        "Hien thi Trong Nuoc",
        "Hien thi Quoc Te",
        "Quay lai"
    };

    while (true) {
        int choice = MenuDieuKhien(mainMenus);
        switch (choice) {
        case 0: ThemDonHang(); break;
        case 1: XoaDonHang(); break;
        case 2:
        {
            while (true) {
                int subChoice = MenuDieuKhien(subMenuHienThi);
                if (subChoice == 0) HienThiDonHang(0);
                else if (subChoice == 1) HienThiDonHang(1);
                else if (subChoice == 2) HienThiDonHang(2);
                else break;
            }
        }
        break;
        case 3: TinhLoiNhuan(); break;
        case 4:
            GhiFile();
            cout << "\nDu lieu da duoc luu vao 2 file: TrongNuoc.txt va QuocTe.txt\n";
            return 0;
        }
    }
    return 0;
}

// ===========================================================================
// PHẦN 3: ĐỊNH NGHĨA CHI TIẾT (IMPLEMENTATION)
// ===========================================================================

// --- GUI IMPLEMENTATION ---
void TextColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void GotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// --- DONHANG IMPLEMENTATION ---
DonHang::DonHang(string ma, string ngay, int goi, double gia) {
    MaDonHang = ma;
    NgayDatHang = ngay;
    GoiVanChuyen = goi;
    GiaTriDonHang = gia;
}

DonHang::DonHang() {
    MaDonHang = "";
    NgayDatHang = "";
    GoiVanChuyen = 1;
    GiaTriDonHang = 0.0;
}

DonHang::~DonHang() {}

void DonHang::setMa(string ma) { MaDonHang = ma; }
void DonHang::setNgay(string ngay) { NgayDatHang = ngay; }
void DonHang::setGoi(int goi) { GoiVanChuyen = goi; }
void DonHang::setGia(double gia) { GiaTriDonHang = gia; }

string DonHang::getMa() { return MaDonHang; }
string DonHang::getNgay() { return NgayDatHang; }
int DonHang::getGoi() { return GoiVanChuyen; }
double DonHang::getGia() { return GiaTriDonHang; }

void DonHang::InThongTin(ostream& os) {
    os << left << setw(15) << MaDonHang
        << setw(15) << NgayDatHang
        << setw(10) << GoiVanChuyen
        << setw(15) << (size_t)GiaTriDonHang;
}

double DonHang::TinhTongTien() {
    return GiaTriDonHang;
}

// --- DONHANGTRONGNUOC IMPLEMENTATION ---
DonHangTrongNuoc::DonHangTrongNuoc() : DonHang() {
    QuangDuong = 0.0;
}

DonHangTrongNuoc::DonHangTrongNuoc(string ma, string ngay, int goi, double gia, double km)
    : DonHang(ma, ngay, goi, gia) {
    QuangDuong = km;
}

void DonHangTrongNuoc::InThongTin(ostream& os) {
    DonHang::InThongTin(os);
    double phiShip = QuangDuong * 8000;
    os << left << setw(15) << QuangDuong
        << setw(15) << (size_t)phiShip
        << setw(15) << (size_t)TinhTongTien();
}

double DonHangTrongNuoc::TinhTongTien() {
    double PhiVanChuyen = QuangDuong * 8000;
    int he_so = 1;
    if (GoiVanChuyen == 2) he_so = 2;
    else if (GoiVanChuyen == 3) he_so = 3;
    return (0.1*GiaTriDonHang + PhiVanChuyen) * he_so;
}

int DonHangTrongNuoc::getType() { return 1; }
double DonHangTrongNuoc::getQuangDuong() { return QuangDuong; } 
void DonHangTrongNuoc::setQuangDuong(double km) { QuangDuong = km; }

istream& operator>>(istream& i, DonHangTrongNuoc& d) {
    cout << "Nhap Ma Don Hang: ";
    getline(i, d.MaDonHang);
    cout << "Nhap Ngay Dat Hang: ";
    getline(i, d.NgayDatHang);
    cout << "Nhap Goi Van Chuyen (1-Co ban, 2-Nhanh, 3-Hoa toc): ";
    i >> d.GoiVanChuyen;
    cout << "Nhap Gia Tri Don Hang: ";
    i >> d.GiaTriDonHang;

    cout << "Nhap Quang Duong (km): ";
    i >> d.QuangDuong;
    i.ignore();
    return i;
}

ostream& operator<<(ostream& o, DonHangTrongNuoc& d) {
    d.InThongTin(o);
    return o;
}

// --- DONHANGQUOCTE IMPLEMENTATION ---
DonHangQuocTe::DonHangQuocTe() : DonHang() {
    ThueHaiQuan = 0.0;
    PhiHaiQuan = 0.0;
    NgayLuuKho = 0;
}

DonHangQuocTe::DonHangQuocTe(string ma, string ngay, int goi, double gia, double thue, double phi, int nlk)
    : DonHang(ma, ngay, goi, gia) {
    ThueHaiQuan = thue;
    PhiHaiQuan = phi;
    NgayLuuKho = nlk;
}

void DonHangQuocTe::InThongTin(ostream& os) {
    DonHang::InThongTin(os);
    os << left << setw(15) << (size_t)ThueHaiQuan
        << setw(15) << (size_t)PhiHaiQuan
        << setw(15) << (size_t)TinhTongTien();
}

double DonHangQuocTe::TinhTongTien() {
    double he_so = 1;
    ThueHaiQuan = 0.1 * GiaTriDonHang;
    if (GoiVanChuyen == 2) he_so = 2;
    else if (GoiVanChuyen == 3) he_so = 3;
    return (GiaTriDonHang + ThueHaiQuan + PhiHaiQuan * NgayLuuKho) * he_so;
}

int DonHangQuocTe::getType() { return 2; }
double DonHangQuocTe::getThue() { return ThueHaiQuan; }
double DonHangQuocTe::getPhi() { return PhiHaiQuan; }
int DonHangQuocTe::getNgay() { return NgayLuuKho; }
void DonHangQuocTe::setThue(double thue) { ThueHaiQuan = thue; }
void DonHangQuocTe::setPhi(double phi) { PhiHaiQuan = phi; }
void DonHangQuocTe::setNgay(int nlk) { NgayLuuKho = nlk; }

istream& operator>>(istream& i, DonHangQuocTe& d) {
    cout << "Nhap Ma Don Hang: ";
    getline(i, d.MaDonHang);
    cout << "Nhap Ngay Dat Hang: ";
    getline(i, d.NgayDatHang);
    cout << "Nhap Goi Van Chuyen (1-Co ban, 2-Nhanh, 3-Hoa toc): ";
    i >> d.GoiVanChuyen;
    cout << "Nhap Gia Tri Don Hang: ";
    i >> d.GiaTriDonHang;

    cout << "Nhap Ngay Luu Kho: ";
    i >> d.NgayLuuKho;
    d.ThueHaiQuan = d.getGia() * 0.1;
    d.PhiHaiQuan = 100000;
    i.ignore();
    return i;
}

ostream& operator<<(ostream& o, DonHangQuocTe& d) {
    d.InThongTin(o);
    return o;
}

// --- CHỨC NĂNG QUẢN LÝ & FILE IMPLEMENTATION ---

void DocFile() {
    dsDonHang.clear();

    // 1. Doc file Trong Nuoc
    ifstream fTN("TrongNuoc.txt");
    if (!fTN.fail()) {
        string ma, ngay;
        int goi; double gia, km;
        while (getline(fTN, ma)) {
            getline(fTN, ngay);
            fTN >> goi >> gia >> km;
            fTN.ignore();

            dsDonHang.push_back(new DonHangTrongNuoc(ma, ngay, goi, gia, km));
        }
        fTN.close();
    }

    // 2. Doc file Quoc Te
    ifstream fQT("QuocTe.txt");
    if (!fQT.fail()) {
        string ma, ngay;
        int goi, nlk; double gia; double thue, phi;
        while (getline(fQT, ma)) {
            getline(fQT, ngay);
            fQT >> goi >> gia >> thue >> phi >> nlk;
            fQT.ignore();

            dsDonHang.push_back(new DonHangQuocTe(ma, ngay, goi, gia, thue, phi, nlk));
        }
        fQT.close();
    }
}

void GhiFile() {
    ofstream fTN("TrongNuoc.txt");
    ofstream fQT("QuocTe.txt");

    for (auto x : dsDonHang) {
        if (x->getType() == 1) {
            DonHangTrongNuoc* d = dynamic_cast<DonHangTrongNuoc*>(x);
            fTN << d->getMa() << endl;
            fTN << d->getNgay() << endl;
            fTN << d->getGoi() << endl;
            fTN << d->getGia() << endl;
            fTN << d->getQuangDuong() << endl;
        }
        else if (x->getType() == 2) {
            DonHangQuocTe* d = dynamic_cast<DonHangQuocTe*>(x);
            fQT << d->getMa() << endl;
            fQT << d->getNgay() << endl;
            fQT << d->getGoi() << endl;
            fQT << d->getGia() << endl;
            fQT << d->getThue() << " " << d->getPhi() << " " << d->getNgay() << endl;
        }
    }
    fTN.close();
    fQT.close();
}

void TieuDeTrongNuoc() {
    TextColor(11);
    cout << "\n--- DON HANG TRONG NUOC ---\n";
    cout << left << setw(15) << "Ma DH"
        << setw(15) << "Ngay"
        << setw(10) << "Goi"
        << setw(15) << "Gia Tri"
        << setw(15) << "Quang Duong"
        << setw(15) << "Phi Van Chuyen"
        << setw(15) << "TONG TIEN VAN CHUYEN" << endl;
    cout << string(100, '-') << endl;
    TextColor(7);
}

void TieuDeQuocTe() {
    TextColor(13);
    cout << "\n--- DON HANG QUOC TE ---\n";
    cout << left << setw(15) << "Ma DH"
        << setw(15) << "Ngay"
        << setw(10) << "Goi"
        << setw(15) << "Gia Tri"
        << setw(15) << "Thue HQ"
        << setw(15) << "Phi HQ"
        << setw(15) << "TONG TIEN" << endl;
    cout << string(100, '-') << endl;
    TextColor(7);
}

void HienThiDonHang(int filter) {
    system("cls");
    cout << "\n=== CHI TIET DON HANG ===\n";

    bool coDuLieu = false;

    if (filter == 0 || filter == 1) {
        int count = 0;
        for (auto x : dsDonHang) if (x->getType() == 1) count++;

        if (count > 0) {
            TieuDeTrongNuoc();
            for (auto x : dsDonHang) {
                if (x->getType() == 1) {
                    x->InThongTin(cout);
                    cout << endl;
                }
            }
            coDuLieu = true;
        }
        else if (filter == 1) cout << "\n(Khong co don hang trong nuoc)\n";
    }

    if (filter == 0 || filter == 2) {
        int count = 0;
        for (auto x : dsDonHang) if (x->getType() == 2) count++;

        if (count > 0) {
            TieuDeQuocTe();
            for (auto x : dsDonHang) {
                if (x->getType() == 2) {
                    x->InThongTin(cout);
                    cout << endl;
                }
            }
            coDuLieu = true;
        }
        else if (filter == 2) cout << "\n(Khong co don hang quoc te)\n";
    }

    if (!coDuLieu && filter == 0) cout << "\nDanh sach trong!\n";

    cout << "\n";
    system("pause");
}

void ThemDonHang() {
    int chon;
    system("cls");
    cout << "1. Them Don hang Trong nuoc\n";
    cout << "2. Them Don hang Quoc te\n";
    cout << "Lua chon cua ban: ";
    cin >> chon;
    cin.ignore();

    DonHang* dh = nullptr;
    if (chon == 1) {
        DonHangTrongNuoc* d = new DonHangTrongNuoc();
        cin >> *d;
        dh = d;
    }
    else if (chon == 2) {
        DonHangQuocTe* d = new DonHangQuocTe();
        cin >> *d;
        dh = d;
    }

    if (dh != nullptr) {
        dsDonHang.push_back(dh);
        cout << "\n-> Them don hang thanh cong!\n";
    }
    system("pause");
}

void XoaDonHang() {
    system("cls");
    cout << "=== CHUC NANG XOA DON HANG ===\n\n";

    TextColor(14);
    cout << "DANH SACH MA DON HANG HIEN CO:\n";
    TextColor(7);
    cout << left << setw(20) << "MA DON" << "LOAI DON HANG" << endl;

    if (dsDonHang.empty()) {
        cout << "(Trong)\n";
    }
    else {
        for (auto x : dsDonHang) {
            cout << left << setw(20) << x->getMa()
                << (x->getType() == 1 ? "Trong nuoc" : "Quoc te") << endl;
        }
    }

    cout << string(40, '-') << endl;

    string maXoa;
    cout << "Nhap Ma Don Hang can xoa: ";

    getline(cin, maXoa);

    int index = -1;
    for (int i = 0; i < dsDonHang.size(); i++) {
        if (dsDonHang[i]->getMa() == maXoa) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        delete dsDonHang[index];
        dsDonHang.erase(dsDonHang.begin() + index);
        TextColor(10);
        cout << "-> Da xoa don hang " << maXoa << " thanh cong!\n";
        TextColor(7);
    }
    else {
        TextColor(12);
        cout << "-> Khong tim thay ma don hang!\n";
        TextColor(7);
    }
    system("pause");
}

void TinhLoiNhuan() {
    system("cls");
    double tongTN = 0, tongQT = 0;
    cout << "\n=== BAO CAO LOI NHUAN ===\n\n";

    for (auto x : dsDonHang) {
        if (x->getType() == 1) tongTN += x->TinhTongTien();
        else tongQT += x->TinhTongTien();
    }

    cout << left << setw(40) << "Tong doanh thu Don hang Trong Nuoc: " << (size_t)tongTN << " VND" << endl;
    cout << left << setw(40) << "Tong doanh thu Don hang Quoc Te: " << (size_t)tongQT << " VND" << endl;
    cout << string(50, '-') << endl;
    TextColor(14);
    cout << left << setw(40) << "TONG CONG: " << (size_t)(tongTN + tongQT) << " VND" << endl;
    TextColor(7);

    cout << "\n";
    system("pause");
}

int MenuDieuKhien(vector<string> menuItems) {
    int pointer = 0;
    int n = (int)menuItems.size();
    while (true) {
        system("cls");

        TextColor(14);
        cout << "CHUONG TRINH QUAN LY VAN CHUYEN\n";
        TextColor(8);
        cout << "(Su dung phim mui ten LEN/XUONG)\n\n";
        TextColor(7);
        for (int i = 0; i < n; ++i) {
            if (i == pointer) {
                TextColor(240);
                cout << " >> " << menuItems[i] << " << " << endl;
                TextColor(7);
            }
            else {
                cout << "    " << menuItems[i] << endl;
            }
        }
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) {
                pointer--;
                if (pointer < 0) pointer = n - 1;
            }
            else if (key == 80) {
                pointer++;
                if (pointer >= n) pointer = 0;
            }
        }
        else if (key == 13) {
            return pointer;
        }
    }
}