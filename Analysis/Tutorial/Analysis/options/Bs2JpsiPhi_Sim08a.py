#-- GAUDI jobOptions generated on Mon Sep 30 12:11:46 2013
#-- Contains event types : 
#--   13144001 - 143 files - 2572489 events - 531.36 GBytes


#--  Extra information about the data processing phases:

from Gaudi.Configuration import * 
from GaudiConf import IOHelper
IOHelper('ROOT').inputFiles(['LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000001_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000002_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000003_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000004_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000005_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000006_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000007_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000008_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000009_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000010_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000011_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000012_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000013_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000014_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000015_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000016_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000017_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000018_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000019_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000020_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000021_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000022_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000023_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000024_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000025_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000026_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000027_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000028_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000029_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000030_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000031_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000032_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000033_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000034_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000035_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000036_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000037_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000038_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000039_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000040_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000041_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000042_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000043_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000044_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000045_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000046_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000047_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000048_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000049_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000050_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000051_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000052_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000053_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000054_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000055_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000056_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000057_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000058_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000059_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000060_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000061_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000062_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000063_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000064_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000065_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000066_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000067_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000068_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000069_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000070_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000071_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000072_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000073_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000074_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000075_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000076_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000077_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000078_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000079_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000080_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000081_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000082_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000083_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000084_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000085_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000086_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000087_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000088_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000089_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000090_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000091_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000092_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000093_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000094_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000095_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000096_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000097_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000098_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000099_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000100_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000101_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000102_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000103_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000104_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000105_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000106_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000107_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000108_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000109_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000110_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000111_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000112_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000113_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000114_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000115_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000116_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000117_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000118_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000119_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000120_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000121_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000122_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000123_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000124_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000125_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000126_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000127_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000128_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000129_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000131_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000132_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000133_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000134_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000135_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000136_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000137_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000138_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000139_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000140_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000141_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000142_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000143_1.allstreams.dst',
'LFN:/lhcb/MC/2012/ALLSTREAMS.DST/00025235/0000/00025235_00000144_1.allstreams.dst'
], clear=True)
FileCatalog().Catalogs += [ 'xmlcatalog_file:$ANALYSISROOT/options/Bs2JpsiPhi_Sim08a.xml' ]
