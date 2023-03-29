import qrcode

# dataM = "Master"
# dataS = "Slave"
data = "https://github.com/Entity014/REAI_Project_Door"

qr = qrcode.QRCode(
        version=1,
        box_size=15,
        error_correction=qrcode.constants.ERROR_CORRECT_H,
        border=1)
qr.add_data(data)
qr.make(fit=True)
img = qr.make_image(fill='black', back_color='white')

# imgM = qrcode.make(dataM)
# img = qrcode.make(dataS)
# img.save('Python file/image/Master.png')
# img.save('Python file/image/Slave.png')
img.save('Python file/image/Data.png')
