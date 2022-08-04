<?php    
$servername = "localhost";
$dbname = "chiasen1_qrcodeproject";
$username = "chiasen1_doanhsang9x";
$password = "2641997@#$";
$errorcode = "";
$api_key_value = "r2i5dPRqC7p2J";
$api_key= $valueRaw = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $valueRaw = test_input($_POST["value"]);
        $value = explode ("|" , $valueRaw);
        $tenhangne = $value[1];
        $ngaynhapxuathangne = $value[3];
        $hansudungne = $value[2];
        if ($value[0] == "nhaphang" or $value[0] == "xuathang") {
            $conn = new mysqli($servername, $username, $password, $dbname);
            // Check connection
            if ($conn->connect_error) {
                $thongbao = "Lỗi kết nối CSDL";
                $errorcode = "203";
                die("Connection failed: " . $conn->connect_error);
            } 
            if ($tenhangne != "" and $ngaynhapxuathangne != "" and $hansudungne != "") {
                $sqli = "SELECT id, soLuong FROM trangThaiHangHoa WHERE tenHang='$tenhangne' and hanSuDung = '$hansudungne'";
                $result = $conn->query($sqli);
                if ($result->num_rows > 0) {
                    while($row = $result->fetch_assoc()) {
                        $soLuongHienCo = $row["soLuong"];
                        $idHangTrongKho = $row["id"];
                    }
                }
                else {
                    $sqlis = "INSERT INTO trangThaiHangHoa (tenHang, hanSuDung) VALUES ('$tenhangne', '$hansudungne')";
                    if ($conn->query($sqlis) === TRUE) {
                        $sqli = "SELECT id, soLuong FROM trangThaiHangHoa WHERE tenHang='$tenhangne' and hanSuDung = '$hansudungne'";
                        $result = $conn->query($sqli);
                        if ($result->num_rows > 0) {
                            while($row = $result->fetch_assoc()) {
                                $soLuongHienCo = $row["soLuong"];
                                $idHangTrongKho = $row["id"];
                            }
                        }
                    } 
                }
            }
            if ($value[0] == "nhaphang") {
                $sql = "SELECT id, soLuong FROM lichSuXuatNhapHang WHERE phanLoai = '$value[0]' and tenHang='$tenhangne' and ngayThucHien='$ngaynhapxuathangne' and hanSuDung = '$hansudungne'";
                $result = $conn->query($sql);
                if ($result->num_rows > 0) {
                    while($row = $result->fetch_assoc()) {
                        $soluong = $row["soLuong"] + 1;
                        $id = $row["id"];
                        $sqls = "UPDATE lichSuXuatNhapHang SET soLuong=$soluong WHERE id=$id";
                        if ($conn->query($sqls) === TRUE) {
                            $thongbao = "(Lịch sử) Cập nhật thêm số lượng thành công (nhập hàng)";
                            $errorcode = "264";
                        } 
                        else {
                            $thongbao = "Lỗi: " . $sqls . "<br>" . $conn->error;
                            $errorcode = "203";
                        }
                    }
                } 
                else {
                    $sqls = "INSERT INTO lichSuXuatNhapHang (phanLoai, tenHang, ngayThucHien, hanSuDung) VALUES ('$value[0]' ,'$tenhangne', '$ngaynhapxuathangne', '$hansudungne')";
                    if ($conn->query($sqls) === TRUE) {
                        $thongbao = "(Lịch sử) Nhập sản phẩm thành công";
                        $errorcode = "264";
                    } 
                    else {
                        $thongbao = "Lỗi: " . $sqls . "<br>" . $conn->error;
                        $errorcode = "203";
                    }
                }
                
            }
            
            if ($value[0] == "xuathang") {
                if ($soLuongHienCo > 0) {
                    $sql = "SELECT id, soLuong FROM lichSuXuatNhapHang WHERE phanLoai = '$value[0]' and tenHang='$tenhangne' and ngayThucHien='$ngaynhapxuathangne' and hanSuDung = '$hansudungne'";
                    $result = $conn->query($sql);
                    if ($result->num_rows > 0) {
                        while($row = $result->fetch_assoc()) {
                            $soluong = $row["soLuong"] + 1;
                            $id = $row["id"];
                            $sqls = "UPDATE lichSuXuatNhapHang SET soLuong=$soluong WHERE id=$id";
                            if ($conn->query($sqls) === TRUE) {
                                $thongbao = "(Lịch sử) Cập nhật thêm số lượng thành công (xuất hàng)";
                                $errorcode = "264";
                            } 
                            else {
                                $thongbao = "Lỗi: " . $sqls . "<br>" . $conn->error;
                                $errorcode = "203";
                            }
                        }
                    } 
                    else {
                        $sqls = "INSERT INTO lichSuXuatNhapHang (phanLoai, tenHang, ngayThucHien, hanSuDung) VALUES ('$value[0]' ,'$tenhangne', '$ngaynhapxuathangne', '$hansudungne')";
                        if ($conn->query($sqls) === TRUE) {
                            $thongbao = "(Lịch sử) Xuất sản phẩm thành công";
                            $errorcode = "264";
                        } 
                        else {
                            $thongbao = "Lỗi: " . $sqls . "<br>" . $conn->error;
                            $errorcode = "203";
                        }
                    }
                }
                else {
                    $thongbao = "Sản phẩm này không còn trong kho, không thể xuất hàng";
                    $errorcode = "203";
                }
            }            
            if ($value[0] == "xuathang") {
                if ($soLuongHienCo > 0) {
                    $soLuongHienCo = $soLuongHienCo - 1;
                    $sqlss = "UPDATE trangThaiHangHoa SET soLuong=$soLuongHienCo WHERE id=$idHangTrongKho";
                    if ($conn->query($sqlss) === TRUE) {
                        $thongbao = "Cập nhật số lượng hàng trong kho thành công!";
                        $errorcode = "264";
                    }
                    else {
                        $thongbao = "Lỗi: " . $sqlss . "<br>" . $conn->error;
                        $errorcode = "203";
                    }
                }
            }
            elseif ($value[0] == "nhaphang") {
                $soLuongHienCo = $soLuongHienCo + 1;
                $sqlss = "UPDATE trangThaiHangHoa SET soLuong=$soLuongHienCo WHERE id=$idHangTrongKho";
                if ($conn->query($sqlss) === TRUE) {
                    $thongbao = "Cập nhật số lượng hàng trong kho thành công!";
                    $errorcode = "264";
                }
                else {
                    $thongbao = "Lỗi: " . $sqlss . "<br>" . $conn->error;
                    $errorcode = "203";
                }
            }
                
            $conn->close();
        }
    }
    else {
        $thongbao = "Đã cung cấp khóa API sai.";
        $errorcode = "203";
    }
}
else {
    $thongbao = "Không có dữ liệu nào được đăng với HTTP POST.";
    $errorcode = "203";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>
<?php echo $errorcode . "|" . $thongbao ?>