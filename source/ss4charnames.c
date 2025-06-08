#include <3ds/types.h>

/*
	Flags:
	0: None
	1: Not in Contacts
	2: Unseen
	3: Fashion Forward Leftover
*/

const char* ss4CharacterNames[] = {
	"None",
	"Rosie",
	"Yolanda",
	"Alina",
	"Cece",
	"Lorelei",
	"Camilla",
	"Janice",
	"Margot",
	"Niamh",
	"Pippa",
	"Evie", // Unseen
	"Angélique",
	"Angélique (Dupe)", // Unseen
	"Advert Director",
	"??? (Feminine)", // Unseen
	"Interviewer",
	"??? (Lively)", // Unseen
	"Abigail",
	"Benedetta",
	"Olivia",
	"Safia",
	"Jewel",
	"Audrey",
	"Hibiki",
	"Ayala",
	"Adrienne",
	"Xin",
	"Sylvia",
	"Hannah",
	"Jo",
	"Anneke",
	"Madeira",
	"Jessica",
	"Yvette Regnier",
	"Gillian Grace",
	"Tessa Lyle",
	"Yulia Mirova",
	"Kiki Summers",
	"??? (Kiki's Student)", // Not in Contacts
	"Clover",
	"Flora",
	"??? (Girly)", // Unseen
	"??? (Girly)", // Unseen
	"??? (Bold)", // Unseen
	"??? (Feminine)", // Unseen
	"??? (Lively)", // Unseen
	"??? (Gothic)", // Unseen
	"Maya Cadenza", // Photos only
	"Kevani (Dupe)", // Unseen
	"Hafsa",
	"Elsie",
	"Leila",
	"Leila (Dupe)", // Unseen
	"Zara",
	"Kay",
	"Jocelyn",
	"??? (Basic)", // Unseen
	"Tallulah",
	"Mina",
	"Molly",
	"Gwen",
	"Tracy",
	"Chetna",
	"Svetlana",
	"Reiko",
	"Kevani",
	"Erisa",
	"Saya",
	"Tanita",
	"Cornelia",
	"Shamim",
	"Chantelle",
	"Aya",
	"Celia",
	"Frances",
	"Mylene", // Unseen
	"Adrienne", // FF, Unseen
	"Hitomi",
	"Momo",
	"Mikiko",
	"Maru",
	"Erisa", // Used for Yolanda's Songs
	"Saya", // Used for Yolanda's Songs
	"Erisa", // Used for Alina's Songs
	"Saya", // Used for Alina's Songs
	"Zayna", // Unseen
	"Niamh", // FF, Unseen
	"Mimi", // Unseen
	"Flavia", // Unseen
	"Kay", // FF, Unseen
	"Rio", // Unseen
	"Sharla", // FF, Unseen
	"Tallulah", // FF, Unseen
	"Benedetta", // FF, Unseen
};

const char* ss4CharacterNamesP2[] = {
	// Starting at 0x0063
	"??? (Character dummy)",
	"Julie",
	"Victoria",
	"Caoimhe",
	"Naomi",
	"Abbie",
	"Raylene",
	"Inge",
	"Kara",
	"Chikako",
	"Chisato",
	"Katie",
	"Zalia",
	"Tameka",
	"Zadia",
	"Susmita",
	"Sunita",
	"Deepti",
	"Meesh",
	"Zainab",
	"Hikari",
	"Warittha",
	"Eirin",
	"Annie",
	"Gina",
	"Bhavna",
	"Preeya",
	"Genevieve",
	"Melinda",
	"Ameena",
	"Ivy",
};

const char* ss4CharacterNamesP3[] = {
	// Starting at 0x0096
	"Jiamin",
	"Lilia",
	"Sinitta",
	"Reem",
	"Juanita",
	"Imani",
	"Darcie",
	"Misato",
	"Phoebe",
	"Courtney",
	"Toshiko",
	"Aleesha",
	"Mariko",
	"Keana",
	"Jana",
	"Savka",
	"Airi",
	"Binita",
	"Takako",
	"Sharla",
	"Julisa",
	"Violet",
	"Krisha",
	"Afia",
	"Jemima",
	"Roxie",
	"Zuleika",
	"Latisha",
	"Kamala",
	"Aeisha",
};

const char* ss4CharacterNamesP4[] = {
	// Starting at 0x00C8
	"Shayna",
	"Lydia",
	"Renasha",
	"Frida",
	"Juliet",
	"Jem",
	"Willow",
	"Jayda",
	"Shayla",
	"Marjan",
	"Yumiko",
	"Fiona",
	"Jodie",
	"Aliyah",
	"Robyn",
	"Nicole",
	"Charlene",
	"Estelle",
	"Maddy",
	"Kalei",
	"Anjali",
	"Nita",
	"Nettie",
	"Lucia",
	"Jean",
	"Demi",
	"Sam",
	"Fiore",
	"Doris",
	"Yui",
};

const char* ss4CharacterNamesP5[] = {
	// Starting at 0x00FA
	"Harue",
	"Afsana",
	"Tasleem",
	"Tabitha",
	"Aiko",
	"Ann",
	"Tanya",
	"Elysa",
	"Min-Kyung",
	"Kayleigh",
	"Miki",
	"Anastasia",
	"Siobhan",
	"Shauna",
	"Tilly",
	"Imogen",
	"Petunia",
	"Shenice",
	"Belinda",
	"Emma",
	"Shilpa",
	"Jenna",
	"Dani",
	"Krissi",
	"Li",
	"Polly",
	"Alice",
	"Christina",
	"Huiting",
	"Nellie",
};

const char* ss4CharacterNamesP6[] = {
	// Starting at 0x012C
	"Cecilia",
	"Coletta",
	"Juno",
	"Helen",
	"Jenn",
	"June",
	"Kareena",
	"Bushra",
	"Charity",
	"Sakura",
	"Jill",
	"Kiran",
	"Rita",
	"Priscilla",
	"Sabine",
	"Shelly",
	"Meg",
	"Jolene",
	"Liz",
	"Eleanor",
	"Shaz",
	"K.C.",
	"Martha",
	"Chloe",
	"Paige",
	"Leah",
	"Hope",
	"Shona",
	"Livia",
	"Ashley",
};

const char* ss4CharacterNamesP7[] = {
	// Starting at 0x015E
	"Bonnie",
	"Marjorie",
	"Una",
	"Emmy",
	"Love",
	"Nina",
	"Selma",
	"Yukiko",
	"Glenda",
	"Nikki",
	"Wendy",
	"Verity",
	"Laetitia",
	"Hilary",
	"Bethan",
	"Emiliana",
	"Fifi",
	"Amira",
	"Gabby",
	"Serena",
	"Rebecca",
	"Lucinda",
	"Bunny",
	"Mareike",
	"Jade",
	"Sun",
	"Brenda",
	"Trinnie",
	"Lauren",
	"Alma",
};

const char* ss4CharacterNamesP8[] = {
	// Starting at 0x0190
	"Aurora",
	"Daphne",
	"Sanam",
	"Angie",
	"Nene",
	"Sonia",
	"Bryony",
	"Rosaline",
	"Kelsie",
	"Terra",
	"Pauline",
	"Terri",
	"Sara",
	"Jasmin",
	"Caroline",
	"Jayne",
	"Miranda",
	"Millie",
	"Tara",
	"May",
	"Carrie",
	"Cathy",
	"Yuki",
	"Natalie",
	"Amaia",
	"Pixie",
	"Lucille",
	"Larissa",
	"Lacey",
	"Liberty",
};

const char* ss4CharacterNamesP9[] = {
	// Starting at 0x01C2
	"Lotte",
	"Chien Yu",
	"Princess",
	"Summer",
	"Melody",
	"Daniella",
	"Rhoda",
	"Bella",
	"Maeve",
	"Davina",
	"Emily",
	"Savannah",
	"Cindy",
	"Elsa",
	"Cheryl",
	"Selena",
	"Morwenna",
	"Annabelle",
	"Valerie",
	"Scarlet",
	"Tia",
	"Dorothea",
	"Kristin",
	"Talia",
	"Bobbie",
	"Briana",
	"Erin",
	"Nisha",
	"Amara",
	"Effie",
};

const char* ss4CharacterNamesP10[] = {
	// Starting at 0x01F4
	"Nadia",
	"Matilda",
	"Sandra",
	"Jacqui",
	"Kimberly",
	"Sybil",
	"Lily",
	"Jamie-Lee",
	"Lorina",
	"Krystal",
	"Colleen",
	"Shirley",
	"Petra",
	"Carmen",
	"Gail",
	"Pollyanna",
	"Martine",
	"Alexis",
	"Cerise",
	"Tammy",
	"Portia",
	"Saffron",
	"Heather",
	"Nicolette",
	"Franny",
	"April",
	"Annett",
	"Roxanne",
	"Ruth",
	"Vicky",
};

//const char* ss4CharacterNamesP11[] = {
	// Starting at 0x0226
	// Same as ss3CharacterNamesP12
//};

const char* ss4CharacterNamesP12[] = {
	// Starting at 0x0258
	"Harmony",
	"Liu",
	"Eddie",
	"Dahlia",
	"Megan",
	"Sheena",
	"Heidi",
	"Shira",
	"Sue",
	"Francesca",
	"Lindsay",
	"Nadine",
	"River",
	"Lou",
	"Amelia",
	"Georgie",
	"Ava",
	"Trudy",
	"Daisy",
	"Karen",
	"Tamsin",
	"Ding",
	"Corinne",
	"Jerri",
	"Nikita",
	"Missy",
	"Désirée",
	"Cherie",
	"Blanca",
	"Gerty",
};

const char* ss4CharacterNamesP13[] = {
	// Starting at 0x028A
	"Kyong",
	"Rowan",
	"Marie",
	"Keira",
	"Cassidy",
	"Cordelia",
	"Leoni",
	"Sachi",
	"Lorna",
	"Leire",
	"Paula",
	"Anna",
	"Claudia",
	"Britta",
	"Janine",
	"Mona",
	"Pearl",
	"Maxine",
	"Isolde",
	"Stacey",
	"Tonia",
	"Susanne",
	"Ambra",
	"Myra",
	"Sheila",
	"Maisie",
	"Andrea",
	"Donna",
	"Caitlin",
	"Sadie",
};

//const char* ss4CharacterNamesP14[] = {
	// Starting at 0x02BC
	// Same as ss3CharacterNamesP15
//};

const char* ss4CharacterNamesP15[] = {
	// Starting at 0x02EE
	"Kelly",
	"Glenn",
	"Vivienne",
	"Peggy",
	"Lucy",
	"Natasha",
	"Pru",
	"Gita",
	"Sapphire",
	"Doreen",
	"Veronica",
	"Melissa",
	"Tricia",
	"Cynthia",
	"Jenny",
	"Libby",
	"Angelica",
	"Astra",
	"Malika",
	"Lara",
	"Faith",
	"Sharon",
	"Anila",
	"Mariah",
	"Mirabel",
	"Mavis",
	"Janet",
	"Ella",
	"Hazel",
	"Beryl",
};

const char* ss4CharacterNamesReps[] = {
	// Starting at 0x0320
	"??? (Feminine)", // Unseen
	"Penny",
	"Eliza",
	"Ginger",
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"Freddie",
	"Gabi", // Fashion Forward
	"Erika",
	"Patti",
	"Kez",
	"Karly",
	"Amita",
	"Rhianna",
	"Connie",
	"Padma",
	"Lisa",
	"Aisling",
	"Lettie",
	"Olga",
	"Fliss",
	"Eliza (Royal)", // Fashion Forward
	"Eliza (Pirate)", // Fashion Forward
	"Eliza (Ninja)", // Fashion Forward
	"Eliza (Bee)", // Fashion Forward
	"Eliza (Preppy)", // Fashion Forward
	"??? (Chic)", // Unseen
	"??? (Rock)", // Unseen
	"???", // Unseen
};

const char* ss4CharacterNamesMale[] = {
	// Starting at 0x03E8
	"Ethan",
	"Tim",
	"Florian",
	"Oliver",
	"Johann",
	"Fortman",
	"Tamwar",
	"Announcer",
	"Michel",
	"Melvin",
	"Xiaobai",
	"Director",
	"??? (Basic)", // Unseen
	"Designer",
	"Director",
	"??? (Boho-Chic)", // Unseen
	"Galliard",
	"Mike",
	"Jake",
	"Harvey",
	"Dan",
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"Ken",
	"Archie",
	"Rahil",
	"Vincent",
	"Richard",
	"Heath",
};

const char* ss4CharacterNamesMaleP2[] = {
	// Starting at 0x041A
	"Jim",
	"Blake", // Unseen
	"Daisuke", // FF, Unseen
	"Daisuke",
	"Evan (JAP)", // Unseen
	"Alan", // Unseen
	"Tristan (JAP)", // Unseen
	"Masaru", // Unseen
	"Lewis", // Unseen
	"Ronald", // Unseen
	"Sadiq", // FF, Unseen
	"Tommy", // Unseen
	"Phillip", // Unseen
	"Alex", // Unseen
	"Yuji", // Unseen
	"Colin", // Unseen
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"???", // Unseen
	"Bradley",
	"Jesse",
	"Zakir",
};

const char* ss4CharacterNamesMaleP3[] = {
	// Starting at 0x044C
	"Ruihang",
	"Alistair",
	"Will",
	"Aziz",
	"Jun",
	"Joe",
	"Koji",
	"Owen",
	"Martin",
	"Derek",
	"Steve",
	"Earl",
	"Jeff",
	"Paul",
	"Fabrizio",
	"Tony",
	"Greg",
	"Graham",
	"Henry",
	"Geraint",
	"Luke",
	"Hugh",
	"Ted",
	"Arthur",
	"Sanjay",
	"Joel",
	"Lee",
	"Mitchell",
	"Matt",
	"Jay",
};

const char* ss4CharacterNamesInst[] = {
	// Starting at 0x05DC
	"Scott",
	"Sadiq",
};

const char* ss4CharacterNames09C5[] = {
	"Sophie", // Unseen
	"Callie", // Unseen
};

const char* ss4CharacterNames0A29[] = {
	"Ricky", // Unseen
	"Tarquin (JAP)", // Unseen
};

const char* ss4CharacterNames0B55[] = {
	"Kirsty", // Unseen
};

const int ss4CharacterFlags[] = {
	0, // None
	0, // Rosie
	0, // Yolanda
	0, // Alina
	0, // Cece
	0, // Lorelei
	0, // Camilla
	0, // Janice
	0, // Margot
	0, // Niamh
	0, // Pippa
	3, // Evie
	0, // Angélique
	2, // Angélique (Dupe)
	1, // Advert Director
	2, // Unseen (Feminine)
	1, // Interviewer
	2, // Unseen (Lively)
	0, // Abigail
	0, // Benedetta
	0, // Olivia
	0, // Safia
	0, // Jewel
	0, // Audrey
	0, // Hibiki
	0, // Ayala
	0, // Adrienne
	0, // Xin
	0, // Sylvia
	0, // Hannah
	0, // Jo
	0, // Anneke
	0, // Madeira
	0, // Jessica
	0, // Yvette Regnier
	0, // Gillian Grace
	0, // Tessa Lyle
	0, // Yulia Mirova
	0, // Kiki Summers
	1, // ??? (Kiki's Student)
	0, // Clover
	0, // Flora
	2, // Unseen (Girly)
	2, // Unseen (Girly)
	2, // Unseen (Bold)
	2, // Unseen (Feminine)
	2, // Unseen (Lively)
	2, // Unseen (Gothic)
	1, // Maya Cadenza
	2, // Kevani (Dupe)
	0, // Hafsa
	0, // Elsie
	0, // Leila
	2, // Leila (Dupe)
	0, // Zara
	0, // Kay
	0, // Jocelyn
	2, // Unseen (Basic)
	0, // Tallulah
	0, // Mina
	0, // Molly
	0, // Gwen
	1, // Tracy
	1, // Chetna
	0, // Svetlana
	0, // Reiko
	0, // Kevani
	0, // Erisa
	0, // Saya
	0, // Tanita
	0, // Cornelia
	0, // Shamim
	0, // Chantelle
	0, // Aya
	0, // Celia
	0, // Frances
	3, // Mylene
	3, // Adrienne (FF)
	0, // Hitomi
	0, // Momo
	0, // Mikiko
	0, // Maru
	1, // Erisa (Dupe #1)
	1, // Saya (Dupe #1)
	1, // Erisa (Dupe #2)
	1, // Saya (Dupe #2)
	3, // Zayna
	3, // Niamh (FF)
	3, // Mimi
	3, // Flavia
	3, // Kay (FF)
	3, // Rio
	3, // Sharla (FF)
	3, // Tallulah (FF)
	3, // Benedetta (FF)
};

const int ss4CharacterFlagsP2[] = {
	1, // ??? (Character dummy)
	0, // Julie
	0, // Victoria
	0, // Caoimhe
	0, // Naomi
	0, // Abbie
	0, // Raylene
	0, // Inge
	0, // Kara
	0, // Chikako
	0, // Chisato
	0, // Katie
	0, // Zalia
	0, // Tameka
	0, // Zadia
	0, // Susmita
	0, // Sunita
	0, // Deepti
	0, // Meesh
	0, // Zainab
	0, // Hikari
	0, // Warittha
	0, // Eirin
	0, // Annie
	0, // Gina
	0, // Bhavna
	0, // Preeya
	0, // Genevieve
	0, // Melinda
	0, // Ameena
	0, // Ivy
};

const int ss4CharacterFlagsReps[] = {
	2, // "??? (Feminine)",
	0, // "Penny",
	0, // "Eliza",
	0, // "Ginger",
	0, // "???",
	0, // "???",
	0, // "???",
	0, // "Freddie",
	3, // "Gabi",
	0, // "Erika",
	0, // "Patti",
	0, // "Kez",
	0, // "Karly",
	0, // "Amita",
	0, // "Rhianna",
	0, // "Connie",
	0, // "Padma",
	0, // "Lisa",
	0, // "Aisling",
	0, // "Lettie",
	0, // "Olga",
	0, // "Fliss",
	3, // "Eliza (FF, Royal)",
	3, // "Eliza (FF, Pirate)",
	3, // "Eliza (FF, Ninja)",
	3, // "Eliza (FF, Bee)",
	3, // "Eliza (FF, Preppy)",
	0, // "??? (Chic)",
	0, // "??? (Rock)",
	0, // "???",
};

const int ss4CharacterFlagsMale[] = {
	0, // Ethan
	0, // Tim
	0, // Florian
	0, // Oliver
	0, // Johann
	0, // Fortman
	0, // Tamwar
	1, // Announcer
	0, // Michel
	0, // Melvin
	0, // Xiaobai
	1, // Director
	0, // ??? (Basic)
	1, // Designer
	1, // Director
	2, // ??? (Boho-Chic)
	0, // Galliard
	0, // Mike
	0, // Jake
	0, // Harvey
	0, // Dan
	2, // ???
	2, // ???
	2, // ???
	2, // ???
	2, // ???
	2, // ???
	0, // Ken
	0, // Archie
	0, // Rahil
	0, // Vincent
	0, // Richard
	0, // Heath
};

const int ss4CharacterFlagsMaleP2[] = {
	0, // Jim
	3, // Blake
	3, // Daisuke
	0, // Daisuke
	3, // Evan (JAP)
	3, // Alan
	3, // Tristan (JAP)
	3, // Masaru
	3, // Lewis
	3, // Ronald
	3, // Sadiq
	3, // Tommy
	3, // Phillip
	3, // Alex
	3, // Yuji
	3, // Colin
	2, // ???
	2, // ???
	2, // ???
	2, // ???
	2, // ???
	2, // ???
	2, // ???
	2, // ???
	2, // ???
	2, // ???
	2, // ???
	0, // Bradley
	0, // Jesse
	0, // Zakir
};

const int ss4CharacterFlags30_0[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

const int ss4CharacterFlags30_3[] = {
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
};

const u16 ss4CharacterOrder_AtoB[] = {
	0x0068, // Abbie
	0x0012, // Abigail
	0x001A, // Adrienne
	0x004D, // Adrienne (FF) (Unseen)
	0x000E, // Advert Director
	0x00B3, // Aeisha
	0x00AD, // Afia
	0x00FB, // Afsana
	0x00FE, // Aiko
	0x00A6, // Airi
	0x0332, // Aisling
	0x02C4, // Alba (Unseen)
	0x00A1, // Aleesha
	0x0427, // Alex (Unseen)
	0x0205, // Alexis
	0x0114, // Alice
	0x0003, // Alina
	0x044D, // Alistair
	0x00D5, // Aliyah
	0x017B, // Alma
	0x01A8, // Amaia
	0x01DE, // Amara
	0x02A0, // Ambra
	0x0080, // Ameena
	0x0266, // Amelia
	0x02CB, // Amelle (Unseen)
	0x016F, // Amira
	0x032D, // Amita
	0x0105, // Anastasia
	0x02A4, // Andrea
	0x02FE, // Angelica
	0x000C, // Angélique
	0x0193, // Angie
	0x0304, // Anila
	0x00DC, // Anjali
	0x022E, // Anke
	0x00FF, // Ann
	0x0295, // Anna
	0x01D3, // Annabelle
	0x001F, // Anneke
	0x007A, // Annie
	0x020E, // Annett
	0x03EF, // Announcer
	0x020D, // April
	0x0404, // Archie
	0x0463, // Arthur
	0x0149, // Ashley
	0x02FF, // Astra
	0x0017, // Audrey
	0x0190, // Aurora
	0x0268, // Ava
	0x0049, // Aya
	0x0019, // Ayala
	0x044F, // Aziz
	0x010C, // Belinda
	0x01C9, // Bella
	0x0013, // Benedetta
	0x005E, // Benedetta (FF) (Unseen)
	0x030B, // Beryl
	0x016C, // Bethan
	0x007C, // Bhavna
	0x00A7, // Binita
	0x041B, // Blake (Unseen)
	0x0274, // Blanca
	0x01DA, // Bobbie
	0x015E, // Bonnie
	0x0435, // Bradley
	0x0178, // Brenda
	0x01DB, // Briana
	0x0297, // Britta
	0x023B, // Bronwyn
	0x0196, // Bryony
	0x0174, // Bunny
	0x0133, // Bushra
};

const u16 ss4CharacterOrder_CtoD[] = {
	0x02A6, // Caitlin
	0x09C6, // Callie (Unseen)
	0x0066, // Caoimhe
	0x0006, // Camilla
	0x02D5, // Candace (Unseen)
	0x023F, // Caramel
	0x0201, // Carmen
	0x019E, // Caroline
	0x01A4, // Carrie
	0x028E, // Cassidy
	0x0354, // Catherine (Unseen)
	0x01A5, // Cathy
	0x0004, // Cece
	0x012C, // Cecilia
	0x004A, // Celia
	0x0206, // Cerise
	0x0048, // Chantelle
	0x0063, // Character dummy (Unseen)
	0x0134, // Charity
	0x00D8, // Charlene
	0x0232, // Chelsea
	0x0273, // Cherie
	0x01D0, // Cheryl
	0x003F, // Chetna
	0x01C3, // Chien Yu
	0x006C, // Chikako
	0x006D, // Chisato
	0x0143, // Chloe
	0x0115, // Christina
	0x01CE, // Cindy
	0x0296, // Claudia
	0x0226, // Cleo
	0x0028, // Clover
	0x012D, // Coletta
	0x0429, // Colin (Unseen)
	0x01FE, // Colleen
	0x032F, // Connie
	0x028F, // Cordelia
	0x026E, // Corinne
	0x0046, // Cornelia
	0x009F, // Courtney
	0x02C2, // Courtney (FF) (Unseen)
	0x02FB, // Cynthia
	0x025B, // Dahlia
	0x041D, // Daisuke
	0x041C, // Daisuke (FF) (Unseen)
	0x026A, // Daisy
	0x03FC, // Dan
	0x0110, // Dani
	0x01C7, // Daniella
	0x0191, // Daphne
	0x009C, // Darcie
	0x01CB, // Davina
	0x02C3, // Debs (Unseen)
	0x0074, // Deepti
	0x00E1, // Demi
	0x0231, // Denise
	0x0455, // Derek
	0x03F5, // Designer
	0x0272, // Désirée
	0x023D, // Diana
	0x026D, // Ding
	0x03F3, // Director
	0x03F6, // Director
	0x02D8, // Dolly (Unseen)
	0x02A5, // Donna
	0x02F7, // Doreen
	0x00E4, // Doris
	0x01D7, // Dorothea
};

const u16 ss4CharacterOrder_EtoF[] = {
	0x0457, // Earl
	0x025A, // Eddie
	0x01DF, // Effie
	0x0079, // Eirin
	0x013F, // Eleanor
	0x0322, // Eliza
	0x0336, // Eliza (FF, Royal) (Unseen)
	0x0337, // Eliza (FF, Pirate) (Unseen)
	0x0338, // Eliza (FF, Ninja) (Unseen)
	0x0339, // Eliza (FF, Bee) (Unseen)
	0x033A, // Eliza (FF, Preppy) (Unseen)
	0x0309, // Ella
	0x0352, // Ellie (Unseen)
	0x0357, // EL-LI-OT (Unseen)
	0x01CF, // Elsa
	0x0033, // Elsie
	0x02C6, // Elsie (FF) (Unseen)
	0x0101, // Elysa
	0x016D, // Emiliana
	0x01CC, // Emily
	0x010D, // Emma
	0x0161, // Emmy
	0x0329, // Erika
	0x01DC, // Erin
	0x0043, // Erisa
	0x00D9, // Estelle
	0x03E8, // Ethan
	0x000B, // Evie (Unseen)
	0x02BC, // Fabienne (Unseen)
	0x045A, // Fabrizio
	0x0302, // Faith
	0x0233, // Fern
	0x016E, // Fifi
	0x00D3, // Fiona
	0x00E3, // Fiore
	0x0059, // Flavia (Unseen)
	0x0335, // Fliss
	0x0238, // Flo
	0x0029, // Flora
	0x03EA, // Florian
	0x03ED, // Fortman
	0x004B, // Frances
	0x0261, // Francesca
	0x020C, // Franny
	0x0327, // Freddie
	0x00CB, // Frida
};

const u16 ss4CharacterOrder_GtoI[] = {
	0x0170, // Gabby
	0x0328, // Gabi (Unseen)
	0x0202, // Gail
	0x03F8, // Galliard
	0x007E, // Genevieve
	0x0267, // Georgie
	0x045F, // Geraint
	0x0275, // Gerty
	0x02C9, // Gillian (FF) (Unseen)
	0x0023, // Gillian Grace
	0x007B, // Gina
	0x0323, // Ginger
	0x02F5, // Gita
	0x0166, // Glenda
	0x02EF, // Glenn
	0x045D, // Graham
	0x045C, // Greg
	0x003D, // Gwen
	0x0032, // Hafsa
	0x001D, // Hannah
	0x0258, // Harmony
	0x00FA, // Harue
	0x03FB, // Harvey
	0x0234, // Hayley
	0x030A, // Hazel
	0x0408, // Heath
	0x020A, // Heather
	0x025E, // Heidi
	0x012F, // Helen
	0x045E, // Henry
	0x0077, // Hikari
	0x016B, // Hilary
	0x004E, // Hitomi
	0x0146, // Hope
	0x0461, // Hugh
	0x0116, // Huiting
	0x009B, // Imani
	0x0109, // Imogen
	0x006A, // Inge
	0x0010, // Interviewer
	0x029C, // Isolde
	0x0081, // Ivy
};

const u16 ss4CharacterOrder_JtoL[] = {
	0x01F7, // Jacqui
	0x0176, // Jade
	0x03FA, // Jake
	0x01FB, // Jamie-Lee
	0x00A4, // Jana
	0x0308, // Janet
	0x0007, // Janice
	0x0298, // Janine
	0x019D, // Jasmin
	0x02D6, // Jasna (Unseen)
	0x0469, // Jay
	0x00CF, // Jayda
	0x019F, // Jayne
	0x00E0, // Jean
	0x0458, // Jeff
	0x00CD, // Jem
	0x00AE, // Jemima
	0x0130, // Jenn
	0x010F, // Jenna
	0x02FC, // Jenny
	0x026F, // Jerri
	0x0436, // Jesse
	0x0021, // Jessica
	0x0096, // Jiamin
	0x0136, // Jill
	0x041A, // Jim
	0x001E, // Jo
	0x0038, // Jocelyn
	0x00D4, // Jodie
	0x0451, // Joe
	0x0465, // Joel
	0x03EC, // Johann
	0x013D, // Jolene
	0x009A, // Juanita
	0x0227, // Judy
	0x02D3, // Julia (Unseen)
	0x022D, // Juliana
	0x0064, // Julie
	0x00CC, // Juliet
	0x00AA, // Julisa
	0x0450, // Jun
	0x0131, // June
	0x012E, // Juno
	0x0141, // K.C.
	0x00DB, // Kalei
	0x00B2, // Kamala
	0x006B, // Kara
	0x0132, // Kareena
	0x026B, // Karen
	0x032C, // Karly
	0x006E, // Katie
	0x022B, // Katrina
	0x0240, // Katya
	0x0037, // Kay
	0x005A, // Kay (FF) (Unseen)
	0x0103, // Kayleigh
	0x00A3, // Keana
	0x028D, // Keira
	0x02EE, // Kelly
	0x0198, // Kelsie
	0x0403, // Ken
	0x032B, // Kez
	0x0042, // Kevani
	0x0026, // Kiki Summers
	0x0027, // ??? (Kiki's Student)
	0x02C1, // Kim (Unseen)
	0x01F8, // Kimberly
	0x0137, // Kiran
	0x0B55, // Kirsty (Unseen)
	0x0359, // Kitty (Unseen)
	0x0452, // Koji
	0x00AC, // Krisha
	0x0111, // Krissi
	0x01D8, // Kristin
	0x01FD, // Krystal
	0x028A, // Kyong
	0x01AC, // Lacey
	0x016A, // Laetita
	0x0301, // Lara
	0x01AB, // Larissa
	0x00B1, // Latisha
	0x017A, // Lauren
	0x0145, // Leah
	0x0466, // Lee
	0x0034, // Leila
	0x0293, // Leire
	0x02D7, // Lena (Unseen)
	0x035A, // Lenore (Unseen)
	0x0290, // Leoni
	0x0355, // Leonora (Unseen)
	0x0333, // Lettie
	0x0112, // Li
	0x02FD, // Libby
	0x01AD, // Liberty
	0x0097, // Lilia
	0x01FA, // Lily
	0x02BE, // Lin (Unseen)
	0x0262, // Lindsay
	0x0331, // Lisa
	0x0259, // Liu
	0x0148, // Livia
	0x013E, // Liz
	0x0358, // Lord B (Unseen)
	0x0005, // Lorelei
	0x01FC, // Lorina
	0x0292, // Lorna
	0x02CE, // Lorraine (Unseen)
	0x01C2, // Lotte
	0x0265, // Lou
	0x0162, // Love
	0x022A, // Luana
	0x00DF, // Lucia
	0x01AA, // Lucille
	0x0173, // Lucinda
	0x02F2, // Lucy
	0x0241, // Luisa
	0x0460, // Luke
	0x00C9, // Lydia
};

const u16 ss4CharacterOrder_MtoN[] = {
	0x02CA, // Mabel (Unseen)
	0x00DA, // Maddy
	0x0020, // Madeira
	0x01CA, // Maeve
	0x02C8, // Maggie (Unseen)
	0x02A3, // Maisie
	0x0300, // Malika
	0x02D4, // Marcia (Unseen)
	0x0175, // Mareike
	0x0008, // Margot
	0x0305, // Mariah
	0x028C, // Marie
	0x00A2, // Mariko
	0x0229, // Marina
	0x00D1, // Marjan
	0x015F, // Marjorie
	0x0142, // Martha
	0x0454, // Martin
	0x0204, // Martine
	0x0051, // Maru
	0x0235, // Mary
	0x0421, // Masaru (Unseen)
	0x01F5, // Matilda
	0x0468, // Matt
	0x0230, // Maureen
	0x0307, // Mavis
	0x029B, // Maxine
	0x01A3, // May
	0x0030, // Maya Cadenza
	0x0075, // Meesh
	0x013C, // Meg
	0x025C, // Megan
	0x023E, // Melanie
	0x007F, // Melinda
	0x02BF, // Melinda (FF) (Unseen)
	0x02F9, // Melissa
	0x01C6, // Melody
	0x03F1, // Melvin
	0x0237, // Mica
	0x03F0, // Michell
	0x03F9, // Mike
	0x0104, // Miki
	0x0050, // Mikiko
	0x02D0, // Milla (Unseen)
	0x01A1, // Millie
	0x0058, // Mimi (Unseen)
	0x0102, // Min-Kyung
	0x0306, // Mirabel
	0x01A0, // Miranda
	0x009D, // Misato
	0x0271, // Missy
	0x0467, // Mitchel
	0x02D1, // Moira (Unseen)
	0x003C, // Molly
	0x004F, // Momo
	0x0299, // Mona
	0x023A, // Monica
	0x0242, // Morganna
	0x01D2, // Morwenna
	0x02D9, // Muriel (Unseen)
	0x004C, // Mylene (Unseen)
	0x02A1, // Myra
	0x01F4, // Nadia
	0x0263, // Nadine
	0x0067, // Naomi
	0x01A7, // Natalie
	0x02F3, // Natasha
	0x0117, // Nellie
	0x0194, // Nene
	0x00DE, // Nettie
	0x0009, // Niamh
	0x0057, // Niamh (FF) (Unseen)
	0x00D7, // Nicole
	0x020B, // Nicolette
	0x0270, // Nikita
	0x0167, // Nikki
	0x0163, // Nina
	0x01DD, // Nisha
	0x00DD, // Nita
};

const u16 ss4CharacterOrder_OtoP[] = {
	0x0334, // Olga
	0x03EB, // Oliver
	0x0014, // Olivia
	0x0453, // Owen
	0x0330, // Padma
	0x0144, // Paige
	0x023C, // Patsy
	0x032A, // Patti
	0x0459, // Paul
	0x0294, // Paula
	0x019A, // Pauline
	0x029A, // Pearl
	0x02F1, // Peggy
	0x0321, // Penny
	0x022C, // Pepita
	0x0200, // Petra
	0x010A, // Petunia
	0x0228, // Philippa
	0x0426, // Philip (Unseen)
	0x009E, // Phoebe
	0x000A, // Pippa
	0x01A9, // Pixie
	0x0113, // Polly
	0x0203, // Pollyanna
	0x02D2, // Poppy (Unseen)
	0x0208, // Portia
	0x007D, // Preeya
	0x01C4, // Princess
	0x0139, // Priscilla
	0x02F4, // Pru
};

const u16 ss4CharacterOrder_QtoS[] = {
	0x0405, // Rahil
	0x0069, // Raylene
	0x0172, // Rebecca
	0x0099, // Reem
	0x0041, // Reiko
	0x00CA, // Renasha
	0x032E, // Rhianna
	0x01C8, // Rhoda
	0x02BD, // Rhonda (Unseen)
	0x0407, // Richard
	0x0A29, // Ricky (Unseen)
	0x005B, // Rio (Unseen)
	0x0138, // Rita
	0x0264, // River
	0x00D6, // Robyn
	0x0423, // Ronald (Unseen)
	0x0197, // Rosaline
	0x0001, // Rosie
	0x028B, // Rowan
	0x020F, // Roxanne
	0x00AF, // Roxie
	0x044C, // Ruihang
	0x0210, // Ruth
	0x013A, // Sabine
	0x02CD, // Sabrina (Unseen)
	0x0291, // Sachi
	0x02A7, // Sadie
	0x05DD, // Sadiq
	0x0424, // Sadiq (FF) (Unseen)
	0x0209, // Saffron
	0x0135, // Sakura
	0x0356, // Sally (Unseen)
	0x00E2, // Sam
	0x02C0, // Samantha (Unseen)
	0x0192, // Sanam
	0x01F6, // Sandra
	0x0464, // Sanjay
	0x02F6, // Sapphire
	0x019C, // Sara
	0x01CD, // Savannah
	0x00A5, // Savka
	0x0044, // Saya
	0x01D5, // Scarlet
	0x05DC, // Scott
	0x01D1, // Selena
	0x0164, // Selma
	0x0171, // Serena
	0x0047, // Shamim
	0x00A9, // Sharla
	0x005C, // Sharla (FF) (Unseen)
	0x0303, // Sharon
	0x0107, // Shauna
	0x00D0, // Shayla
	0x00C8, // Shayna
	0x0140, // Shaz
	0x025D, // Sheena
	0x02A2, // Sheila
	0x013B, // Shelly
	0x010B, // Shenice
	0x010E, // Shilpa
	0x025F, // Shira
	0x01FF, // Shirley
	0x0147, // Shona
	0x0106, // Siobhan
	0x0098, // Sinitta
	0x0195, // Sonia
	0x09C5, // Sophie (Unseen)
	0x029D, // Stacey
	0x0456, // Steve
	0x02CC, // Stevie (Unseen)
	0x0260, // Sue
	0x01C5, // Summer
	0x0177, // Sun
	0x0073, // Sunita
	0x029F, // Susanne
	0x0243, // Susie
	0x0072, // Susmita
	0x0040, // Svetlana
	0x01F9, // Sybil
	0x001C, // Sylvia
};

const u16 ss4CharacterOrder_TtoV[] = {
	0x00FD, // Tabitha
	0x00A8, // Takako
	0x01D9, // Talia
	0x003A, // Tallulah
	0x005D, // Tallulah (FF) (Unseen)
	0x0070, // Tameka
	0x0207, // Tammy
	0x026C, // Tamsin
	0x03EE, // Tamwar
	0x0045, // Tanita
	0x0100, // Tanya
	0x01A2, // Tara
	0x0A2A, // Tarquin (JAP) (Unseen)
	0x00FC, // Tasleem
	0x022F, // Tatiana
	0x02CF, // Taylor (Unseen)
	0x0462, // Ted
	0x0239, // Tegan
	0x02C7, // Teresa (Unseen)
	0x0199, // Terra
	0x019B, // Terri
	0x0024, // Tessa Lyle
	0x01D6, // Tia
	0x0108, // Tilly
	0x03E9, // Tim
	0x0425, // Tommy (Unseen)
	0x029E, // Tonia
	0x045B, // Tony
	0x00A0, // Toshiko
	0x003E, // Tracy
	0x02FA, // Tricia
	0x0236, // Trina
	0x0179, // Trinnie
	0x0353, // Trixie (Unseen)
	0x0269, // Trudy
	0x0160, // Una
	0x01D4, // Valerie
	0x0169, // Verity
	0x02F8, // Veronica
	0x0211, // Vicky
	0x0065, // Victoria
	0x0406, // Vincent
	0x00AB, // Violet
	0x02F0, // Vivienne
};

const u16 ss4CharacterOrder_WtoZ[] = {
	0x0078, // Warittha
	0x0168, // Wendy
	0x044E, // Will
	0x00CE, // Willow
	0x02C5, // Winifred (Unseen)
	0x03F2, // Xiaobai
	0x001B, // Xin
	0x0002, // Yolanda
	0x00E5, // Yui
	0x0428, // Yuji (Unseen)
	0x01A6, // Yuki
	0x0165, // Yukiko
	0x0025, // Yulia Mirova
	0x00D2, // Yumiko
	0x0022, // Yvette Reginer
	0x0071, // Zadia
	0x0437, // Zakir
	0x006F, // Zalia
	0x0076, // Zaniab
	0x0036, // Zara
	0x0056, // Zayna (Unseen)
	0x00B0, // Zuleika
};