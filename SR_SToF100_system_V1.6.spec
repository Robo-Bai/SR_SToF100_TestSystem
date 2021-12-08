# -*- mode: python ; coding: utf-8 -*-

block_cipher = None


a = Analysis(['SR_SToF100_system_V1.6.py',
			  'D:\\SR_SToF100_TestSystemV1.5\\drivers\\pcan\\PCANBasic.py',
			  'D:\\SR_SToF100_TestSystemV1.5\\Main_code\\CSV_Plot.py',
			  'D:\\SR_SToF100_TestSystemV1.5\\Main_code\\CSV_write.py',
			  'D:\\SR_SToF100_TestSystemV1.5\\Ui\\Data_display.py',
			  'D:\\SR_SToF100_TestSystemV1.5\\Ui\\Label.py',
			  'D:\\SR_SToF100_TestSystemV1.5\\Ui\\location.py',
			  'D:\\SR_SToF100_TestSystemV1.5\\Ui\\MainUi.py',
			  'D:\\SR_SToF100_TestSystemV1.5\\Ui\\MotorControl.py',
			  'D:\\SR_SToF100_TestSystemV1.5\\Ui\\Version.py',
			  'D:\\SR_SToF100_TestSystemV1.5\\ProjectPath.py'],
             pathex=['D:\\SR_SToF100_TestSystemV1.5'],
             binaries=[],
             datas=[('D:\\SR_SToF100_TestSystemV1.5\\Icon\\sr.jpg','.\Icon'),
					('D:\\SR_SToF100_TestSystemV1.5\\Matplot_data\\Distance_data.csv','.\Matplot_data'),
					('D:\\SR_SToF100_TestSystemV1.5\\Matplot_data\\Distance_image.png','.\Matplot_data'),
					('D:\\SR_SToF100_TestSystemV1.5\\Matplot_data\\outliersRate.csv','.\Matplot_data'),
					('D:\\SR_SToF100_TestSystemV1.5\\Matplot_data\\OutliersRate_image.png','.\Matplot_data'),
					('D:\\SR_SToF100_TestSystemV1.5\\Matplot_data\\Strenght_image.png','.\Matplot_data'),
					('D:\\SR_SToF100_TestSystemV1.5\\Matplot_data\\Strength_data.csv','.\Matplot_data'),
					('D:\\SR_SToF100_TestSystemV1.5\\ttf\\msyh.ttc','.'),
					('D:\\Anaconda3\\Lib\\site-packages\\reportlab\\fonts\\msyh.ttc','.')],
             hiddenimports=['matplotlib',
							'pkg_resources.py2_warn',
							'MySQLdb',
							'psycopg2'],
             hookspath=[],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher,
             noarchive=False)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          [],
          exclude_binaries=True,
          name='SR_SToF100_system_V1.6',
          debug=False,
          bootloader_ignore_signals=False,
          strip=False,
          upx=True,
          console=True)
coll = COLLECT(exe,
               a.binaries,
               a.zipfiles,
               a.datas,
               strip=False,
               upx=True,
               upx_exclude=[],
               name='SR_SToF100_system_V1.6')