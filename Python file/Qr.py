import qrcode

dataM = "Master"
# dataS = "Slave"

qr = qrcode.QRCode(
        version=1,
        box_size=15,
        error_correction=qrcode.constants.ERROR_CORRECT_H,
        border=1)
qr.add_data(dataM)
qr.make(fit=True)
img = qr.make_image(fill='black', back_color='white')

# imgM = qrcode.make(dataM)
# img = qrcode.make(dataS)
img.save('REAI-Pro/image/Master.png')
# img.save('REAI-Pro/image/Slave.png')
