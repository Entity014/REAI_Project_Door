import qrcode

# dataM = "Master"
dataS = "Slave"

qr = qrcode.QRCode(
        version=1,
        box_size=10,
        border=2)
qr.add_data(dataS)
qr.make(fit=True)
img = qr.make_image(fill='black', back_color='white')

# imgM = qrcode.make(dataM)
# imgS = qrcode.make(dataS)
# img.save('Master.png')
img.save('Slave.png')