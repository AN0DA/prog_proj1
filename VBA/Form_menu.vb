Option Compare Database



Private Sub button_stolowka_swietlica_Click()

Dim ID As Long
Dim Temp As Variant
Dim sSQL As String
    
    'pobranie ID ucznia
ID = InputBox("Podaj ID ucznia, którego przeprowadzamy do œwietlicy", "stó³ówka -> œwietlica")
    
    ' sprawdzenie, czy jest na stolówce
Temp = DLookup("[id_ucznia]", "obecnie_stolowka", "id_ucznia = " & ID)
If Temp <> ID Then
    MsgBox "Na sto³ówce nie ma ucznia o ID = " & ID, "Brak rekordu"
    Exit Sub
End If
ID = CLng(Temp)

        'potwierdzenie tozsamosci
Temp = MsgBox(Funkcje_globalne.Znajdz_ucznia_id(ID), vbYesNo, "Czy to dobra osoba?")
If Temp <> vbYes Then
    Exit Sub
End If

            'dodanie do swietlicy
sSQL = "INSERT INTO obecnie_swietlica (ID_ucznia)" & vbCrLf & _
    "VALUES (" & ID & ");"
CurrentDb.Execute sSQL, dbFailOnError + dbSeeChanges
            'usuniecie ze stolówki
sSQL = "DELETE FROM obecnie_stolowka WHERE ID_ucznia = " & ID & ";"
CurrentDb.Execute sSQL, dbFailOnError + dbSeeChanges

Me.Refresh

Funkcje_globalne.Wyswietl_info ("Przeniesiono ucznia ID = " & ID & " do œwietlicy")

End Sub

        
Private Sub button_swietlica_stolowka_Click()

Dim ID As Long
Dim Temp As Variant
Dim sSQL As String

'pobranie ID ucznia
ID = InputBox("Podaj ID ucznia, którego wyprowadzamy na sto³ówkê", "œwietlica -> stó³ówka")

            'sprawdzenie, czy jest w swietlicy
Temp = DLookup("[id_ucznia]", "obecnie_swietlica", "id_ucznia = " & ID)
If Temp <> ID Then
    MsgBox "W œwietlicy nie ma ucznia o ID = " & ID, "Brak rekordu"
    Exit Sub
End If

  'potwierdzenie tozsamosci
Temp = MsgBox(Funkcje_globalne.Znajdz_ucznia_id(ID), vbYesNo, "Czy to dobra osoba?")
If Temp <> vbYes Then
    Exit Sub
End If

    'dodanie do stolówki
sSQL = "INSERT INTO obecnie_stolowka (ID_ucznia)" & vbCrLf & _
    "VALUES (" & ID & ");"
CurrentDb.Execute sSQL, dbFailOnError + dbSeeChanges
    'usuniecie ze swietlicy
sSQL = "DELETE FROM obecnie_swietlica WHERE ID_ucznia = " & ID & ";"
CurrentDb.Execute sSQL, dbFailOnError + dbSeeChanges

Me.Refresh

Funkcje_globalne.Wyswietl_info ("Przeniesiono ucznia ID = " & ID & " do sto³ówki")
End Sub


Private Sub Etykieta10_Click()
Dim Temp As Variant

Temp = Funkcje_globalne.Update_liczba()

Me.Refresh
End Sub

Private Sub Etykieta6_Click()
Dim Temp As Variant

Temp = Funkcje_globalne.Update_liczba()

Me.Refresh

End Sub

Private Sub list_button_click()

Dim Temp As Variant
Dim Kod As Long
Dim List As Long
Dim Time As String

'pobranie kodu ucznia
Temp = InputBox("Podaj KOD", "Dodaj list")
'wyjœcie kiedy cancel
On Error GoTo 0
If Temp = "" Then
    Exit Sub
End If
Kod = CLng(Temp)

'czy KOD w bazie
Temp = DLookup("[kod]", "uczniowie", "kod = " & Kod)
'exit kiedy KOD nie w bazie
If Temp <> Kod Then
    Temp = MsgBox("Brak rekordu dla KODU = " & Kod, vbOK, "Brak rekordu -- dodaj list")
    Exit Sub
End If


' Pobranie kodu listu
Temp = InputBox("Podaj ID bohatera", "Dodaj list")
'wyjœcie kiedy cancel
On Error GoTo 0
If Temp = "" Then
    Exit Sub
End If
List = CLng(Temp)

'czy ofiara w bazie
Temp = DLookup("[ID_ofiary]", "ofiary", "ID_ofiary = " & List)
'exit kiedy ofiara nie w bazie
If Temp <> List Then
    Temp = MsgBox("Brak ofiary dla ID = " & List, vbOK, "Brak rekordu -- dodaj list")
    Exit Sub
End If


'pobranie danych ofiary [ID, nazwa]
Temp = DLookup("[ID_ofiary]", "ofiary", "ID_ofiary = " & List)
sSQL = DLookup("[nazwa]", "ofiary", "ID_ofiary = " & List)

'potwierdzenie tozsamosci
Temp = MsgBox(Funkcje_globalne.Znajdz_ucznia(Kod) & vbNewLine & vbNewLine & "Bohater:" & vbNewLine & "ID = " & Temp & vbNewLine & sSQL, vbYesNo, "Czy to dobra osoba i bohater? -- dodaj list")
If Temp <> vbYes Then
    Exit Sub
End If


Temp = InputBox("Ile listów?", "Dodaj list", 1)
Time = Hour(Now) & ":" & Minute(Now) & ":" & Second(Now)
ID = DLookup("[ID_ucznia]", "uczniowie", "kod = " & Kod)
sSQL = "INSERT INTO listy (ID_ucznia, ID_ofiary, godzina) VALUES (" & ID & ", " & List & ", '" & Time & "');"

For i = 1 To Temp
    CurrentDb.Execute sSQL, dbFailOnError + dbSeeChanges
Next
Temp = DLookup("[nazwa]", "ofiary", "ID_ofiary = " & List)

Me.Refresh

Funkcje_globalne.Wyswietl_info ("Dodano " & Temp & " listów przez ID = " & ID & " dla " & Temp)

CurrentDb.Execute "DELETE FROM stat_klasa", dbFailOnError + dbSeeChanges
   CurrentDb.Execute "INSERT INTO stat_klasa ( klasa , ilosc) SELECT uczniowie.klasa, Count(listy.ID_listy) as Ilosc FROM listy INNER JOIN uczniowie ON listy.ID_ucznia = uczniowie.ID_ucznia GROUP BY uczniowie.klasa ORDER BY Count(listy.ID_listy) DESC;", dbFailOnError + dbSeeChanges

    CurrentDb.Execute "UPDATE stat_ofiara SET ilosc =" & DCount("[ID_listy]", "listy", "ID_ofiary = 1") & " WHERE ofiara = 1;", dbFailOnError + dbSeeChanges
    CurrentDb.Execute "UPDATE stat_ofiara SET ilosc =" & DCount("[ID_listy]", "listy", "ID_ofiary = 2") & " WHERE ofiara = 2;", dbFailOnError + dbSeeChanges
    CurrentDb.Execute "UPDATE stat_ofiara SET ilosc =" & DCount("[ID_listy]", "listy", "ID_ofiary = 3") & " WHERE ofiara = 3;", dbFailOnError + dbSeeChanges
    CurrentDb.Execute "UPDATE stat_ofiara SET ilosc =" & DCount("[ID_listy]", "listy", "ID_ofiary = 4") & " WHERE ofiara = 4;", dbFailOnError + dbSeeChanges
    CurrentDb.Execute "UPDATE stat_ofiara SET ilosc =" & DCount("[ID_listy]", "listy", "ID_ofiary = 5") & " WHERE ofiara = 5;", dbFailOnError + dbSeeChanges
    CurrentDb.Execute "UPDATE stat_ofiara SET ilosc =" & DCount("[ID_listy]", "listy", "ID_ofiary = 6") & " WHERE ofiara = 6;", dbFailOnError + dbSeeChanges
    CurrentDb.Execute "UPDATE stat_ofiara SET ilosc =" & DCount("[ID_listy]", "listy", "ID_ofiary = 7") & " WHERE ofiara = 7;", dbFailOnError + dbSeeChanges
    CurrentDb.Execute "UPDATE stat_ofiara SET ilosc =" & DCount("[ID_listy]", "listy", "ID_ofiary = 8") & " WHERE ofiara = 8;", dbFailOnError + dbSeeChanges
    CurrentDb.Execute "UPDATE stat_ofiara SET ilosc =" & DCount("[ID_listy]", "listy", "ID_ofiary = 9") & " WHERE ofiara = 9;", dbFailOnError + dbSeeChanges
    CurrentDb.Execute "UPDATE stat_ofiara SET ilosc =" & DCount("[ID_listy]", "listy", "ID_ofiary = 10") & " WHERE ofiara = 10;", dbFailOnError + dbSeeChanges

End Sub


Private Sub wejscie_button_Click()

Dim Temp As Variant
Dim Kod As Long
Dim sSQL As String
Dim Time As String
Dim Nowy As Boolean

'sprawdzanie, czy jest miejsce w swietlicy
Temp = DCount("[ID_ucznia]", "obecnie_swietlica")
If Temp >= Pojemnosc Then
    Temp = MsgBox("Œwietlica jest wype³niona. Wejœæ mimo to?", vbExclamation + vbYesNo, "Œwietlica pe³na")
End If
If Temp = vbNo Then
    Exit Sub
End If

' Pobranie kodu od petenta
Temp = InputBox("Podaj KOD")
'wyjœcie kiedy cancel
On Error GoTo 0
If Temp = "" Then
    Exit Sub
End If
Kod = CLng(Temp)
'czy KOD w bazie
Temp = DLookup("[kod]", "uczniowie", "kod = " & Kod)

'dodawanie nowej osoby jezeli nie istnieje
If Temp = Kod Then
 Debug.Print "temp = kod"
Else
    Nowy = True
    Temp = MsgBox("Brak rekordu dla KODU = " & Kod & vbNewLine & "Dodaæ nowego ucznia?", vbInformation + vbYesNo, "Brak rekordu")
End If

   If Nowy = True And Temp = vbNo Then
        Exit Sub
    End If
    If Nowy = True And Temp = vbYes Then
        KOD_global = Kod
        DoCmd.OpenForm "dodaj_ucznia", , , , , acDialog
        Do Until Pauza = False
            
        Loop
End If

sSQL = DLookup("[id_ucznia]", "uczniowie", "kod = " & Kod)
Temp = DLookup("id_ucznia", "frekwencja", "id_ucznia = " & sSQL & " And godzina_wy is null")
If Temp = sSQL Then
    MsgBox "Uczeñ ju¿ wszed³!", vbCritical + vbOKOnly, "Uczeñ ju¿ wszed³!"
    Exit Sub
End If



'autoryzacja osoby
Temp = MsgBox(Funkcje_globalne.Znajdz_ucznia(Kod), vbOKCancel, "Czy to dobra osoba?")
If Temp = vbCancel Then
    Exit Sub
End If


'godzina, ID_ucznia, kod SQL dodaj¹cy rekord
Time = Hour(Now) & ":" & Minute(Now) & ":" & Second(Now)
ID = DLookup("[ID_ucznia]", "uczniowie", "kod = " & Kod)
sSQL = "INSERT INTO frekwencja (ID_ucznia, godzina_we) " & vbCrLf & _
        "VALUES (" & ID & ", '" & Time & "');"

CurrentDb.Execute sSQL, dbFailOnError + dbSeeChanges

'wybór miejsca, gdzie osoba pisze
Temp = MsgBox("W œwietlicy czy na sto³ówce? [TAK - œwietlica // NIE - stó³ówka]", vbYesNo, "Œwietlica / sto³ówka")
'dodanie do miejsca
If Temp = vbYes Then
    sSQL = "INSERT INTO obecnie_swietlica (ID_ucznia)" & vbCrLf & _
    "VALUES (" & ID & ");"
    CurrentDb.Execute sSQL, dbFailOnError + dbSeeChanges
    Temp = DCount("[ID_ucznia]", "obecnie_swietlica")
    Me.Etykieta30.Caption = Temp & "/" & Pojemnosc
Else
    sSQL = "INSERT INTO obecnie_stolowka (ID_ucznia)" & vbCrLf & _
    "VALUES (" & ID & ");"
    CurrentDb.Execute sSQL, dbFailOnError + dbSeeChanges
    Temp = DCount("[ID_ucznia]", "obecnie_stolowka")
    Me.Etykieta33.Caption = Temp
End If

Me.Refresh

Funkcje_globalne.Wyswietl_info ("Uczeñ ID = " & ID & " wszed³ o " & Time)

End Sub


Private Sub wyjscie_button_Click()

Dim Kod As Long
Dim Temp As Variant
Dim ID As String
Dim Time As String

' Pobranie kodu od petenta
Temp = InputBox("Podaj KOD")
'wyjœcie kiedy cancel
On Error GoTo 0
If Temp = "" Then
    Exit Sub
End If
Kod = CLng(Temp)

Temp = DLookup("[kod]", "uczniowie", "kod = " & Kod)
If Temp = Kod Then
 Debug.Print "temp = kod"
Else
    Temp = MsgBox("Brak rekordu dla KODU = " & Kod, vbCritical + vbOKOnly, "Brak rekordu")
    Exit Sub
End If

'autoryzacja osoby
Temp = MsgBox(Funkcje_globalne.Znajdz_ucznia(Kod), vbOKCancel, "Czy to dobra osoba?")
If Temp = vbCancel Then
    Exit Sub
End If


ID = DLookup("[ID_ucznia]", "uczniowie", "kod = " & Kod)
Temp = DLookup("[ID_frekwencja]", "frekwencja", "ID_ucznia = " & ID & " And godzina_wy is null")
Time = Hour(Now) & ":" & Minute(Now) & ":" & Second(Now)
'dodanie godziny wyjscia we frekwencji
sSQL = "UPDATE frekwencja" & vbCrLf & _
        "SET godzina_wy = '" & Time & "'" & vbCrLf & _
        "WHERE ID_frekwencja = " & Temp & ";"

CurrentDb.Execute sSQL, dbFailOnError + dbSeeChanges

Funkcje_globalne.Wyswietl_info ("Uczeñ ID = " & ID & " wyszed³ o " & Time)

'usuniecie ze swietlicy/stolówki
Temp = DLookup("[ID_ucznia]", "obecnie_swietlica", "ID_ucznia = " & ID)

If Temp = ID Then
    sSQL = "DELETE FROM obecnie_swietlica WHERE ID_ucznia = " & ID & ";"
    CurrentDb.Execute sSQL, dbFailOnError + dbSeeChanges
    Temp = DCount("[ID_ucznia]", "obecnie_swietlica")
    Me.Etykieta30.Caption = Temp & "/" & Pojemnosc
Else
    sSQL = "DELETE FROM obecnie_stolowka WHERE ID_ucznia = " & ID & ";"
    CurrentDb.Execute sSQL, dbFailOnError + dbSeeChanges
    Temp = DCount("[ID_ucznia]", "obecnie_stolowka")
    Me.Etykieta33.Caption = Temp
End If

Me.Refresh
End Sub


