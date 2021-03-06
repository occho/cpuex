library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
entity fsqrt_table is
  port (
		clka : in std_logic;
		addra : in std_logic_vector(9 downto 0);
		douta : out std_logic_vector(35 downto 0) := (others=>'0'));
end fsqrt_table;

architecture op of fsqrt_table is

	subtype table_rec_t is std_logic_vector(35 downto 0);
	type table_t is array (0 to 1023) of table_rec_t;
	signal addr_in	: integer range 0 to 1023;

	constant table : table_t := (
x"6A09E969E",
x"6A6461698",
x"6ABEC1693",
x"6B190D68D",
x"6B7341687",
x"6BCD5D682",
x"6C276567C",
x"6C8155677",
x"6CDB2F671",
x"6D34F366C",
x"6D8EA3666",
x"6DE83B661",
x"6E41BD65B",
x"6E9B29656",
x"6EF481650",
x"6F4DC364B",
x"6FA6EF645",
x"700003640",
x"70590363B",
x"70B1EF635",
x"710AC5630",
x"71638362B",
x"71BC31625",
x"7214C5620",
x"726D4561B",
x"72C5AF616",
x"731E07610",
x"73764760B",
x"73CE73606",
x"74268B601",
x"747E8F5FB",
x"74D67D5F6",
x"752E555F1",
x"7586195EC",
x"75DDC95E7",
x"7635635E2",
x"768CE95DD",
x"76E45D5D7",
x"773BBB5D2",
x"7793055CD",
x"77EA3B5C8",
x"78415B5C3",
x"7898675BE",
x"78EF615B9",
x"7946455B4",
x"799D175AF",
x"79F3D35AA",
x"7A4A7D5A5",
x"7AA1115A0",
x"7AF79359B",
x"7B4E01596",
x"7BA459592",
x"7BFA9F58D",
x"7C50D3588",
x"7CA6F1583",
x"7CFCFD57E",
x"7D52F7579",
x"7DA8DB574",
x"7DFEAB570",
x"7E546B56B",
x"7EAA17566",
x"7EFFAF561",
x"7F553355C",
x"7FAAA5558",
x"800003553",
x"80554F54E",
x"80AA8754A",
x"80FFAF545",
x"8154C1540",
x"81A9C353B",
x"81FEAF537",
x"82538B532",
x"82A85552D",
x"82FD09529",
x"8351AD524",
x"83A63D520",
x"83FABD51B",
x"844F29516",
x"84A381512",
x"84F7CB50D",
x"854BFF509",
x"85A023504",
x"85F431500",
x"8648314FB",
x"869C1D4F7",
x"86EFF94F2",
x"8743C14EE",
x"8797794E9",
x"87EB1D4E5",
x"883EB14E0",
x"8892314DC",
x"88E5A14D7",
x"8938FF4D3",
x"898C4B4CF",
x"89DF854CA",
x"8A32AF4C6",
x"8A85C74C1",
x"8AD8CB4BD",
x"8B2BBF4B9",
x"8B7EA54B4",
x"8BD1754B0",
x"8C24354AC",
x"8C76E54A7",
x"8CC9834A3",
x"8D1C0F49F",
x"8D6E8B49B",
x"8DC0F7496",
x"8E134F492",
x"8E659748E",
x"8EB7CD48A",
x"8F09F5485",
x"8F5C0B481",
x"8FAE0F47D",
x"900003479",
x"9051E5475",
x"90A3B9470",
x"90F57B46C",
x"91472B468",
x"9198CD464",
x"91EA5B460",
x"923BDB45C",
x"928D49458",
x"92DEA9453",
x"932FF744F",
x"93813544B",
x"93D261447",
x"94237F443",
x"94748B43F",
x"94C58743B",
x"951673437",
x"95674F433",
x"95B81B42F",
x"9608D742B",
x"965983427",
x"96AA1F423",
x"96FAAB41F",
x"974B2741B",
x"979B93417",
x"97EBEF413",
x"983C3B40F",
x"988C7940B",
x"98DCA5407",
x"992CC3403",
x"997CCD400",
x"99CCCD3FC",
x"9A1CBB3F8",
x"9A6C993F4",
x"9ABC693F0",
x"9B0C293EC",
x"9B5BD93E8",
x"9BAB7B3E4",
x"9BFB0B3E0",
x"9C4A8D3DD",
x"9C99FF3D9",
x"9CE9633D5",
x"9D38B73D1",
x"9D87FD3CD",
x"9DD7313CA",
x"9E26573C6",
x"9E756F3C2",
x"9EC4793BE",
x"9F13713BB",
x"9F625B3B7",
x"9FB1373B3",
x"A000033AF",
x"A04EC13AC",
x"A09D6F3A8",
x"A0EC0F3A4",
x"A13A9F3A1",
x"A1892139D",
x"A1D795399",
x"A225FB395",
x"A2744F392",
x"A2C29738E",
x"A310CF38B",
x"A35EFB387",
x"A3AD17383",
x"A3FB23380",
x"A4492137C",
x"A49713378",
x"A4E4F3375",
x"A532C7371",
x"A5808B36E",
x"A5CE4136A",
x"A61BEB366",
x"A66983363",
x"A6B71135F",
x"A7048D35C",
x"A751FD358",
x"A79F5D355",
x"A7ECB1351",
x"A839F534E",
x"A8872D34A",
x"A8D455347",
x"A92171343",
x"A96E7D340",
x"A9BB7D33C",
x"AA086D339",
x"AA5551335",
x"AAA225332",
x"AAEEED32E",
x"AB3BA732B",
x"AB8851328",
x"ABD4F1324",
x"AC217F321",
x"AC6E0331D",
x"ACBA7931A",
x"AD06E1316",
x"AD5339313",
x"AD9F85310",
x"ADEBC530C",
x"AE37F5309",
x"AE8419306",
x"AED031302",
x"AF1C392FF",
x"AF68332FC",
x"AFB4232F8",
x"B000032F5",
x"B04BD52F2",
x"B0979D2EE",
x"B0E3552EB",
x"B12F012E8",
x"B17AA12E4",
x"B1C6312E1",
x"B211B52DE",
x"B25D2D2DA",
x"B2A8972D7",
x"B2F3F32D4",
x"B33F412D1",
x"B38A852CD",
x"B3D5BB2CA",
x"B420E32C7",
x"B46BFF2C4",
x"B4B70F2C0",
x"B502112BD",
x"B54D052BA",
x"B597ED2B7",
x"B5E2C92B4",
x"B62D992B0",
x"B678592AD",
x"B6C30F2AA",
x"B70DB72A7",
x"B758532A4",
x"B7A2E12A1",
x"B7ED6529D",
x"B837DB29A",
x"B88243297",
x"B8CCA1294",
x"B916F1291",
x"B9613328E",
x"B9AB6B28B",
x"B9F595288",
x"BA3FB5284",
x"BA89C7281",
x"BAD3CD27E",
x"BB1DC527B",
x"BB67B1278",
x"BBB193275",
x"BBFB67272",
x"BC452D26F",
x"BC8EE926C",
x"BCD899269",
x"BD223B266",
x"BD6BD3263",
x"BDB55D260",
x"BDFEDD25D",
x"BE484F25A",
x"BE91B5257",
x"BEDB11254",
x"BF245F251",
x"BF6DA124E",
x"BFB6D724B",
x"C00003248",
x"C04921245",
x"C09233242",
x"C0DB3B23F",
x"C1243523C",
x"C16D25239",
x"C1B607236",
x"C1FEDF233",
x"C247AB230",
x"C2906B22D",
x"C2D91F22A",
x"C321C9227",
x"C36A65224",
x"C3B2F7221",
x"C3FB7B21E",
x"C443F321C",
x"C48C63219",
x"C4D4C5216",
x"C51D1D213",
x"C56569210",
x"C5ADA920D",
x"C5F5DD20A",
x"C63E07207",
x"C68623205",
x"C6CE35202",
x"C7163B1FF",
x"C75E371FC",
x"C7A6271F9",
x"C7EE0D1F6",
x"C835E51F3",
x"C87DB31F1",
x"C8C5751EE",
x"C90D2D1EB",
x"C954D91E8",
x"C99C7B1E5",
x"C9E40F1E3",
x"CA2B991E0",
x"CA73191DD",
x"CABA8F1DA",
x"CB01F71D7",
x"CB49551D5",
x"CB90A71D2",
x"CBD7EF1CF",
x"CC1F2D1CC",
x"CC665D1CA",
x"CCAD851C7",
x"CCF4A11C4",
x"CD3BB31C1",
x"CD82B71BF",
x"CDC9B31BC",
x"CE10A31B9",
x"CE57871B7",
x"CE9E611B4",
x"CEE5311B1",
x"CF2BF71AE",
x"CF72AF1AC",
x"CFB95F1A9",
x"D000031A6",
x"D0469B1A4",
x"D08D2B1A1",
x"D0D3AF19E",
x"D11A2919C",
x"D16097199",
x"D1A6FD196",
x"D1ED55194",
x"D233A3191",
x"D279E918E",
x"D2C02118C",
x"D30651189",
x"D34C75187",
x"D3928F184",
x"D3D89F181",
x"D41EA317F",
x"D4649F17C",
x"D4AA8F179",
x"D4F073177",
x"D5364F174",
x"D57C1F172",
x"D5C1E716F",
x"D607A316C",
x"D64D5516A",
x"D692FD167",
x"D6D899165",
x"D71E2D162",
x"D763B5160",
x"D7A93315D",
x"D7EEA915A",
x"D83411158",
x"D87971155",
x"D8BEC7153",
x"D90413150",
x"D9495314E",
x"D98E8B14B",
x"D9D3B7149",
x"DA18DB146",
x"DA5DF3144",
x"DAA303141",
x"DAE80513F",
x"DB2D0113C",
x"DB71F113A",
x"DBB6D9137",
x"DBFBB5135",
x"DC4089132",
x"DC8551130",
x"DCCA1112D",
x"DD0EC512B",
x"DD5371128",
x"DD9811126",
x"DDDCA9123",
x"DE2137121",
x"DE65BB11E",
x"DEAA3311C",
x"DEEEA311A",
x"DF330B117",
x"DF7767115",
x"DFBBBB112",
x"E00003110",
x"E0444310D",
x"E0887710B",
x"E0CCA3109",
x"E110C7106",
x"E154DF104",
x"E198EF101",
x"E1DCF30FF",
x"E220EF0FD",
x"E264E30FA",
x"E2A8CB0F8",
x"E2ECAB0F5",
x"E3307F0F3",
x"E3744B0F1",
x"E3B80F0EE",
x"E3FBC70EC",
x"E43F770EA",
x"E4831D0E7",
x"E4C6B90E5",
x"E50A4F0E2",
x"E54DD70E0",
x"E591570DE",
x"E5D4D10DB",
x"E6183D0D9",
x"E65BA10D7",
x"E69EFD0D4",
x"E6E24F0D2",
x"E725970D0",
x"E768D70CD",
x"E7AC0D0CB",
x"E7EF390C9",
x"E8325B0C7",
x"E875770C4",
x"E8B8870C2",
x"E8FB8F0C0",
x"E93E8F0BD",
x"E981830BB",
x"E9C46F0B9",
x"EA07530B7",
x"EA4A2D0B4",
x"EA8CFF0B2",
x"EACFC50B0",
x"EB12850AD",
x"EB553B0AB",
x"EB97E70A9",
x"EBDA890A7",
x"EC1D270A4",
x"EC5FB70A2",
x"ECA23F0A0",
x"ECE4BF09E",
x"ED273709B",
x"ED69A3099",
x"EDAC07097",
x"EDEE63095",
x"EE30B5093",
x"EE7301090",
x"EEB54308E",
x"EEF77908C",
x"EF39A908A",
x"EF7BD1087",
x"EFBDEF085",
x"F00003083",
x"F0420F081",
x"F0841107F",
x"F0C60D07C",
x"F107FF07A",
x"F149E7078",
x"F18BC7076",
x"F1CD9F074",
x"F20F6D072",
x"F2513506F",
x"F292F306D",
x"F2D4A706B",
x"F31653069",
x"F357F7067",
x"F39991065",
x"F3DB25062",
x"F41CAF060",
x"F45E2F05E",
x"F49FA705C",
x"F4E11705A",
x"F5227F058",
x"F563DD056",
x"F5A535053",
x"F5E683051",
x"F627C904F",
x"F6690504D",
x"F6AA3904B",
x"F6EB65049",
x"F72C89047",
x"F76DA3045",
x"F7AEB7043",
x"F7EFC1041",
x"F830C503E",
x"F871BD03C",
x"F8B2AF03A",
x"F8F397038",
x"F93477036",
x"F9754F034",
x"F9B61F032",
x"F9F6E7030",
x"FA37A702E",
x"FA785D02C",
x"FAB90B02A",
x"FAF9B1028",
x"FB3A51026",
x"FB7AE9023",
x"FBBB75021",
x"FBFBFB01F",
x"FC3C7901D",
x"FC7CED01B",
x"FCBD5B019",
x"FCFDBF017",
x"FD3E1D015",
x"FD7E71013",
x"FDBEBD011",
x"FDFF0100F",
x"FE3F3F00D",
x"FE7F7300B",
x"FEBF9F009",
x"FEFFC3007",
x"FF3FDF005",
x"FF7FF3003",
x"FFC001000",
x"000002FFE",
x"003FFAFFA",
x"007FE2FF6",
x"00BFBAFF2",
x"00FF84FEE",
x"013F3CFEA",
x"017EE4FE6",
x"01BE7EFE2",
x"01FE08FDE",
x"023D7EFDB",
x"027CEAFD7",
x"02BC44FD3",
x"02FB90FCF",
x"033ACCFCB",
x"0379F8FC7",
x"03B914FC3",
x"03F820FC0",
x"043720FBC",
x"04760EFB8",
x"04B4EEFB4",
x"04F3C0FB0",
x"053280FAD",
x"057132FA9",
x"05AFD6FA5",
x"05EE6CFA1",
x"062CF0F9E",
x"066B68F9A",
x"06A9D0F96",
x"06E828F93",
x"072672F8F",
x"0764AEF8B",
x"07A2DAF88",
x"07E0F8F84",
x"081F08F80",
x"085D08F7D",
x"089AFCF79",
x"08D8E0F75",
x"0916B4F72",
x"09547CF6E",
x"099234F6B",
x"09CFE0F67",
x"0A0D7CF63",
x"0A4B08F60",
x"0A888AF5C",
x"0AC5FAF59",
x"0B035EF55",
x"0B40B2F52",
x"0B7DFAF4E",
x"0BBB32F4B",
x"0BF85EF47",
x"0C357AF44",
x"0C728AF40",
x"0CAF8CF3D",
x"0CEC80F39",
x"0D2964F36",
x"0D663CF33",
x"0DA308F2F",
x"0DDFC2F2C",
x"0E1C74F28",
x"0E5914F25",
x"0E95A6F22",
x"0ED22EF1E",
x"0F0EA6F1B",
x"0F4B12F17",
x"0F8770F14",
x"0FC3BEF11",
x"100002F0D",
x"103C38F0A",
x"107860F07",
x"10B47CF03",
x"10F088F00",
x"112C88EFD",
x"11687CEFA",
x"11A464EF6",
x"11E03CEF3",
x"121C08EF0",
x"1257C6EED",
x"12937AEE9",
x"12CF1EEE6",
x"130AB6EE3",
x"134640EE0",
x"1381C0EDC",
x"13BD32ED9",
x"13F896ED6",
x"1433EEED3",
x"146F38ED0",
x"14AA7AECC",
x"14E5AAEC9",
x"1520D0EC6",
x"155BE8EC3",
x"1596F4EC0",
x"15D1F2EBD",
x"160CE6EBA",
x"1647CEEB6",
x"1682A8EB3",
x"16BD74EB0",
x"16F836EAD",
x"1732EAEAA",
x"176D92EA7",
x"17A82EEA4",
x"17E2BEEA1",
x"181D42E9E",
x"1857BAE9B",
x"189224E98",
x"18CC84E95",
x"1906D6E92",
x"19411EE8F",
x"197B58E8C",
x"19B586E89",
x"19EFAAE86",
x"1A29C0E83",
x"1A63CCE80",
x"1A9DCAE7D",
x"1AD7BEE7A",
x"1B11A4E77",
x"1B4B80E74",
x"1B8550E71",
x"1BBF14E6E",
x"1BF8CCE6B",
x"1C3278E68",
x"1C6C1AE65",
x"1CA5AEE62",
x"1CDF38E5F",
x"1D18B4E5D",
x"1D5226E5A",
x"1D8B8CE57",
x"1DC4E8E54",
x"1DFE38E51",
x"1E377CE4E",
x"1E70B4E4B",
x"1EA9E2E48",
x"1EE302E46",
x"1F1C1AE43",
x"1F5524E40",
x"1F8E24E3D",
x"1FC71AE3A",
x"200002E37",
x"2038E0E35",
x"2071B2E32",
x"20AA7AE2F",
x"20E338E2C",
x"211BE6E2A",
x"21548EE27",
x"218D28E24",
x"21C5BAE21",
x"21FE3EE1E",
x"2236B8E1C",
x"226F26E19",
x"22A78CE16",
x"22DFE4E14",
x"231832E11",
x"235076E0E",
x"2388AEE0B",
x"23C0DAE09",
x"23F8FEE06",
x"243116E03",
x"246922E01",
x"24A126DFE",
x"24D91EDFB",
x"25110ADF9",
x"2548EEDF6",
x"2580C6DF3",
x"25B892DF1",
x"25F056DEE",
x"26280EDEB",
x"265FBADE9",
x"26975EDE6",
x"26CEF6DE3",
x"270684DE1",
x"273E08DDE",
x"277580DDC",
x"27ACF0DD9",
x"27E454DD6",
x"281BAEDD4",
x"2852FEDD1",
x"288A42DCF",
x"28C17EDCC",
x"28F8AEDCA",
x"292FD4DC7",
x"2966F0DC5",
x"299E02DC2",
x"29D50CDBF",
x"2A0C08DBD",
x"2A42FCDBA",
x"2A79E6DB8",
x"2AB0C6DB5",
x"2AE79ADB3",
x"2B1E66DB0",
x"2B5526DAE",
x"2B8BDEDAB",
x"2BC28ADA9",
x"2BF92EDA6",
x"2C2FC8DA4",
x"2C6658DA1",
x"2C9CDCD9F",
x"2CD358D9C",
x"2D09CAD9A",
x"2D4030D98",
x"2D7690D95",
x"2DACE2D93",
x"2DE32ED90",
x"2E196ED8E",
x"2E4FA6D8B",
x"2E85D2D89",
x"2EBBF8D86",
x"2EF210D84",
x"2F2820D82",
x"2F5E28D7F",
x"2F9424D7D",
x"2FCA18D7A",
x"300002D78",
x"3035E2D76",
x"306BBAD73",
x"30A186D71",
x"30D748D6F",
x"310D04D6C",
x"3142B4D6A",
x"31785ED67",
x"31ADFAD65",
x"31E38ED63",
x"32191AD60",
x"324E9CD5E",
x"328414D5C",
x"32B984D59",
x"32EEEAD57",
x"332446D55",
x"335998D53",
x"338EE4D50",
x"33C424D4E",
x"33F95AD4C",
x"342E8AD49",
x"3463B0D47",
x"3498CAD45",
x"34CDE0D42",
x"3502E8D40",
x"3537E8D3E",
x"356CE0D3C",
x"35A1D0D39",
x"35D6B4D37",
x"360B90D35",
x"364064D33",
x"367530D30",
x"36A9F2D2E",
x"36DEAAD2C",
x"371358D2A",
x"374800D27",
x"377C9ED25",
x"37B132D23",
x"37E5BED21",
x"381A42D1F",
x"384EBED1C",
x"38832ED1A",
x"38B798D18",
x"38EBF6D16",
x"39204ED14",
x"39549ED11",
x"3988E4D0F",
x"39BD20D0D",
x"39F154D0B",
x"3A2580D09",
x"3A59A2D07",
x"3A8DBED04",
x"3AC1D0D02",
x"3AF5DAD00",
x"3B29DACFE",
x"3B5DD2CFC",
x"3B91C0CFA",
x"3BC5A8CF8",
x"3BF988CF5",
x"3C2D5ECF3",
x"3C612ACF1",
x"3C94F0CEF",
x"3CC8ACCED",
x"3CFC60CEB",
x"3D300ACE9",
x"3D63AECE7",
x"3D9748CE5",
x"3DCADACE3",
x"3DFE66CE0",
x"3E31E8CDE",
x"3E6562CDC",
x"3E98D2CDA",
x"3ECC3ACD8",
x"3EFF9CCD6",
x"3F32F4CD4",
x"3F6644CD2",
x"3F998ACD0",
x"3FCCCACCE",
x"400002CCC",
x"403330CCA",
x"406658CC8",
x"409976CC6",
x"40CC8CCC4",
x"40FF9ACC2",
x"4132A2CC0",
x"4165A0CBE",
x"419896CBC",
x"41CB84CBA",
x"41FE6ACB8",
x"42314ACB5",
x"426420CB3",
x"4296EECB1",
x"42C9B2CB0",
x"42FC70CAE",
x"432F26CAC",
x"4361D4CAA",
x"43947ACA8",
x"43C718CA6",
x"43F9AECA4",
x"442C3ECA2",
x"445EC4CA0",
x"449142C9E",
x"44C3BAC9C",
x"44F628C9A",
x"452890C98",
x"455AF0C96",
x"458D46C94",
x"45BF96C92",
x"45F1DEC90",
x"462420C8E",
x"465658C8C",
x"468888C8A",
x"46BAB2C88",
x"46ECD4C86",
x"471EECC85",
x"4750FEC83",
x"478308C81",
x"47B50CC7F",
x"47E706C7D",
x"4818FAC7B",
x"484AE6C79",
x"487CCCC77",
x"48AEA8C75",
x"48E07EC73",
x"49124AC72",
x"494410C70",
x"4975CEC6E",
x"49A786C6C",
x"49D936C6A",
x"4A0ADEC68",
x"4A3C7EC66",
x"4A6E18C64",
x"4A9FA8C63",
x"4AD134C61",
x"4B02B6C5F",
x"4B3432C5D",
x"4B65A6C5B",
x"4B9714C59",
x"4BC878C58",
x"4BF9D6C56",
x"4C2B2CC54",
x"4C5C7CC52",
x"4C8DC4C50",
x"4CBF06C4E",
x"4CF03EC4D",
x"4D2170C4B",
x"4D529CC49",
x"4D83C0C47",
x"4DB4DCC45",
x"4DE5F0C44",
x"4E16FEC42",
x"4E4806C40",
x"4E7906C3E",
x"4EA9FEC3C",
x"4EDAEEC3B",
x"4F0BDAC39",
x"4F3CBCC37",
x"4F6D98C35",
x"4F9E6EC33",
x"4FCF3AC32",
x"500002C30",
x"5030C2C2E",
x"50617AC2C",
x"50922AC2B",
x"50C2D4C29",
x"50F378C27",
x"512416C25",
x"5154AAC24",
x"518538C22",
x"51B5C0C20",
x"51E640C1E",
x"5216B8C1D",
x"52472CC1B",
x"527798C19",
x"52A7FEC17",
x"52D85AC16",
x"5308B0C14",
x"533902C12",
x"536948C11",
x"53998CC0F",
x"53C9C8C0D",
x"53F9FCC0B",
x"542A28C0A",
x"545A50C08",
x"548A70C06",
x"54BA88C05",
x"54EA9CC03",
x"551AA8C01",
x"554AAEBFF",
x"557AAABFE",
x"55AAA2BFC",
x"55DA94BFA",
x"560A7CBF9",
x"563A60BF7",
x"566A3CBF5",
x"569A12BF4",
x"56C9E2BF2",
x"56F9AABF0",
x"57296ABEF",
x"575926BED",
x"5788DCBEB",
x"57B888BEA",
x"57E830BE8",
x"5817D0BE7",
x"58476CBE5",
x"587700BE3",
x"58A68CBE2",
x"58D612BE0",
x"590592BDE",
x"59350ABDD",
x"59647EBDB",
x"5993ECBD9",
x"59C350BD8",
x"59F2B0BD6",
x"5A2208BD5",
x"5A515CBD3",
x"5A80A8BD1",
x"5AAFECBD0",
x"5ADF2CBCE",
x"5B0E64BCD",
x"5B3D96BCB",
x"5B6CC2BC9",
x"5B9BE6BC8",
x"5BCB06BC6",
x"5BFA1EBC5",
x"5C2932BC3",
x"5C583EBC1",
x"5C8742BC0",
x"5CB642BBE",
x"5CE53ABBD",
x"5D142EBBB",
x"5D431ABB9",
x"5D71FEBB8",
x"5DA0E0BB6",
x"5DCFB8BB5",
x"5DFE8CBB3",
x"5E2D58BB2",
x"5E5C1EBB0",
x"5E8ADEBAF",
x"5EB998BAD",
x"5EE84EBAB",
x"5F16FABAA",
x"5F45A2BA8",
x"5F7442BA7",
x"5FA2DEBA5",
x"5FD172BA4",
x"600002BA2",
x"602E8ABA1",
x"605D0CB9F",
x"608B88B9E",
x"60B9FEB9C",
x"60E870B9A",
x"6116D8B99",
x"61453EB97",
x"61739AB96",
x"61A1F2B94",
x"61D044B93",
x"61FE90B91",
x"622CD4B90",
x"625B14B8E",
x"62894EB8D",
x"62B782B8B",
x"62E5AEB8A",
x"6313D6B88",
x"6341F6B87",
x"637014B85",
x"639E28B84",
x"63CC38B82",
x"63FA40B81",
x"642846B7F",
x"645642B7E",
x"64843AB7C",
x"64B22CB7B",
x"64E018B79",
x"650DFCB78",
x"653BDCB77",
x"6569B8B75",
x"65978AB74",
x"65C55AB72",
x"65F322B71",
x"6620E6B6F",
x"664EA2B6E",
x"667C5AB6C",
x"66AA0AB6B",
x"66D7B6B69",
x"67055AB68",
x"6732FCB66",
x"676094B65",
x"678E28B64",
x"67BBB8B62",
x"67E93EB61",
x"6816C2B5F",
x"68443EB5E",
x"6871B8B5C",
x"689F28B5B",
x"68CC94B5A",
x"68F9FAB58",
x"69275AB57",
x"6954B6B55",
x"69820AB54",
x"69AF5AB52",
x"69DCA4B51"
);

begin
	prom_sim: process(clka)
	begin
		if rising_edge(clka) then
			addr_in <= conv_integer(addra);
			douta <= table(addr_in);
		end if;
	end process;
end op;
