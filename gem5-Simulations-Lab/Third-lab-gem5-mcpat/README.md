# Αναφορά τρίτου εργαστηρίου

## Ομάδα 12   Στασινός Αλκιβιάδης ΑΕΜ 9214 , Τσόμπος Χρήστος AEM 9096



* **Βήμα 1ο:**
---

   * **Ερώτημα 1**  

Στο 1ο ερώτημα μας ζητήθηκε να πραγματοποιήσουμε μια μικρή ανάλυση σχετικά με την ισχύ που καταναλώνει ένα σύστημα . Την ισχύ αυτή αποτελούν η **_dynamic power_** , η **_short cirquit power_** (η οποία δε θα μας απασχολήσει στο πλαίσιο αυτής της εργασίας) και η **_static power_** , η οποία συναντάται κυρίως με τον όρο **_leakage_**.

**Dynamic power** :   
Πρόκειται για την ισχύ που καταναλώνεται για την φόρτιση και εκφόρτιση των πυκνωτών κατά τις εναλλαγές κατάστασης των τρανζίστορ του συστήματος. Αξίζει να σημειωθεί πως η δυναμική ισχύς εξαρτάται από το **a** , που καλείται **_activity factor_** και καθορίζεται από τα _accesses_ στη μνήμη που πραγματοποιούνται σε ένα κύκλο ρολογιού.

  **Leakage** :   

  Το συγκεκριμένο είδος ισχύος αφορά την καταναλισκόμενη ισχύ που οφείλεται στη διαρροή ρεύματος των τρανζίστορ. Διακρίνεται σε δύο κατηγορίες , που εξαρτώνται από το μέγεθος  αλλά και από την λογική κατάσταση στην οποία βρίσκονται.  

  **i) Subthreshold leakage :**  

  Οφείλεται στο ρεύμα που διαρρέι μεταξύ της πηγής (_Source_) και της εκροής (_Drain_) του τρανζίστορ όταν αυτό βρίσκεται στη λεγόμενη **_off state_** , κατά την οποία θεωρητικά το ρεύμα θα είχε μηδενική τιμή.  

  **ii) Gate leakage :**   

  Προκαλείται από τη διαρροή ρεύματος στην πύλη (_Gate_) του τρανζίστορ , και μπορεί να λάβει πληθώρα τιμών , που εξαρτάται από την κατάσταση του συστήματος.[[1](##cpitype)]

Η ισχύς που καταναλώνεται εξαρτάται σε μεγάλο βαθμό από το πρόγραμμα που θα εκτελεστεί. Η δυναμική ισχύς θα αυξηθεί με την αύξηση του **_activity factor_** , ενώ θα επηρεαστεί και από τη λογική κατάσταση του συστήματος .  Η τελευταία μεταβάλει  σε σημαντικό βαθμό τη στατική ισχύ . Πιο συγκεκριμένα , όταν δυο τρανζίστορ λειτουργούν παράλληλα (κατάσταση 11) η **Subthreshold leakage** λαμβάνει τη μέγιστη τιμή της , ενώ όταν βρίσκονται στην κατάσταση off (00) η τιμή της θα είναι η ελάχιστη.[[2](##cpitype)]

Τέλος, όταν ένα πρόγραμμα εκτελείται σε μεγαλύτερο χρονικό διάστημα είναι προφανές ότι θα σπαταλά περισσότερη ενέργεια. Ωστόσο ο ρυθμός μεταβολής της ενέργειας (ισχύς) δε θα μεταβληθεί , εφόσον αναφερόμαστε στο ίδιο πρόγραμμα.

* **Ερώτημα 2** 

Ο επεξεργαστής με 40 watt καταναλώνει  8 φορές μεγαλύτερη ισχύ από εκείνον των 5 watt. Ωστόσο , η διάρκεια της μπαταρίας εξαρτάται από την ενέργεια που δαπανάται. Επομένως , αρκεί ο πρώτος επεξεργαστής να εκτελεί το ίδιο πρόγραμμα σε χρονικό διάστημα το οποίο είναι μικρότερο του 1/8 του συνολικού χρόνου που απαιτεί ο δεύτερος , έτσι ώστε η συνολική ενέργεια για την ίδια εκτέλεση να είναι μικρότερη.Συνεπώς μπορεί δώσει μεγαλύτερη διάρκεια μπαταρίας . Το mcPat δε μπορεί να δώσει λύση στο πρόβλημά μας καθώς δε μας δίνει το χρόνο εκτέλεσης με αποτέλεσμα να μη γνωρίζουμε το χρόνο εκτέλεσης του προγράμματος.

* **Ερώτημα 3** 

Στη συνέχεια μας ζητήθηκε να συγκρίνουμε τους δύο επεξεργαστές **_Xeon_** και **_Arm A9_** ως προς την ενεργειακή αποδοτικότητα.Για τη σύγκριση αυτή θα χρειαστούμε τα πεδία _Runtime Dynamic_ ,που αποτελεί τη μέση ισχύ που καταναλώνει ο επεξεργαστής κατά τη διάρκεια εκτέλεσης ενός προγράμματος , και _Total Leakage_ .Με τη χρήση του **_McPAT_** πήραμε τα εξής αποτελέσματα :

|    Επεξεργαστής  |   Runtime Dynamic |  Total Leakage| 
| ------------- |:-------------:| :-------: |
|    Xeon       | 72.9199 W     | 36.8319 W | 
|ARM A9         | 2.96053 W     |0.108687 W |          
  
Θεωρώντας ότι ο **_Xeon_** είναι 40 φορές ταχύτερος από τον **_A9_** , αν υποθέσουμε πως ο πρώτος θα τρέξει ένα πρόγραμμα σε κάποιο χρονικό διάστημα **Τ** , ο δεύτερος απαιτεί χρόνο εκτέλεσης **40Τ** .Τονίζεται πως μετά την εκτέλεση του προγράμματος δε διακόπτεται η λειτουργία του συστήματος , που σημαίνει πως ο **_Xeon_** θα εξακολουθεί να καταναλώνει στατική ισχύ για συνολικό χρόνο **40Τ** . Η προϋπόθεση της μη διακοπής λειτουργίας είναι απαραίτητη , καθώς η συνολική ενέργεια θα πρέπει να υπολογιστεί για το ίδιο χρονικό διάστημα . Για τον υπολογισμό της ενέργειας θα εκτελεστούν οι παρακάτω πράξεις:

**_Xeon Energy_ = (Runtime Dynamic)*T+(Total Leakage)*40*T=1546.1959*T Joule**  

**_A9 Energy_ = (Runtime Dynamic+Total Leakage)*40*T=122.76868*T Joule**

Όπως γίνεται αντιληπτό , για τον ίδιο χρόνο λειτουργίας ο **_Xeon_** καταναλώνει περιπου **12.6** περισσότερη ενέργεια ,γεγονός που αποδεικνύει ότι δε μπορεί να είναι πιο **_Energy Efficient_** από τον **_Arm A9_** .[[3](##cpitype)]




* **Βήμα 2ο:**
---

Αρχικά θα πρέπει να χρησιμοποιήσουμε το _script_  **Gem5ToMcPAT.py** για τη δημιουργία των απαραίτητων _XML_ αρχείων που είναι συμβατά με τον **McPAT**. Λόγω του μεγάλου αριθμού των δοκιμών, προς διευκόλυνση δημιουργήθηκε το _python script_ `xmlres.py` το οποίο παίρνει όλα τα αρχεία αποτελεσμάτων του _gem5_ (stats.txt , config.json) , τα μετατρέπει σε XML εκτελόντας τις κατάλληλες εντολές και τέλος εκτελεί τον **McPAT** με τα αρχεία αυτά.


* **Ερώτημα 1**  

   Στο ερώτημα αυτό θα αναλύσουμε τι σημαίνει ο όρος **EDAP** ( Energy Area Delay Product)
   
   * **Energy**  
   Είναι η ενέργεια που καταναλώνει το πρόγραμμα κατα την εκτέλεση του και μπορεί να βρεθεί απο το γινόμενο του `sim_seconds` (απο το stats.txt που παράγει ο _gem5_) και της **ισχύος** δηλαδή Energy = **sim_seconds** x  ( **Runtime Dynamic** + **Total Leakage**). Επιπλέον το αποτέλεσμα αυτό μπορούμε να το αντλήσουμε αυτόματα κάνοντας χρήση του `print_energy.py` που μας δίνεται.
   
   * **Delay** 
   Το Delay είναι ουσιαστικά το αποτέλεσμα `sim_seconds` και το εξάγουμε απο το αρχείο _stats.txt_
   [[4](##cpitype)]
   
   * **Area**  
   Το Area κομμάτι μπορούμε εύκολα να το εντοπίσουμε απο τα αποτελέσματα του **McPAT** , συγκεκριμένα βρίσκουμε το συνολικό Area αθροίζοντας τα επιμέρους Area του **Core** και της **L2**.
   
   
   

* **Ερώτημα 2** 

Στο ερώτημα αυτό παρουσιάζονται γραφήματα για το Peak **Power** και το συνολικό **Area** μεταβάλλοντας κάθε φορά μόνο μία παράμετρο απο τις πιθανές ενός Cache configuration ( Associativity,Block size etc.). Τα παρακάτω γραφήματα αντικατοπτρίζουν την επίδραση των παραμέτρων αυτών.

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/Blocksize.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/L1Dsize.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/L1Isize.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/L2size.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/L1Dassoc.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/L1Iassoc.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/L2assoc.png" height="420" width="800">

</p>






**Σημαντική σημείωση**:    
Όπως αντιληφθήκαμε εκτελόντας τον McPAT με τα configurations του τελευταίου ερωτήματος του **Εργαστηρίου 2** (τα optimal-cost που βγαίνουν απο τη συνάρτηση συσχέτισης) , μεγάλος αριθμός απο αυτά δεν έβγαζαν αποτέλεσμα απο τον McPAT ( σχεδόν κενά αρχεία txt).Συγκεκριμένα , δίνοντας αρχικά την εντολή `ls optimalcostfiles/txts | wc -l`  για να δούμε το πλήθος των αρχείων παιρνουμε το αποτέλεσμα **465** , ενώ δίνοντας την εντολή `du -a -b --threshold=1000 optimalcostfiles/txts | wc -l` για να μετρήσουμε πόσα αρχεία είναι τουλάχιστον 1ΚΒ παίρνουμε το αποτέλεσμα **187**.Δηλαδή, **40.2 %** των αρχείων δε λειτούργησαν με τον McPAT.

Επιπλέον λόγω του παραπάνω ,καθώς και του ότι δε προβλέψαμε πως το **Cache Line Size** επηρεάζει τόσο έντονα το _Area_ οπότε και το _EDAP_,οι μετρήσεις αυτές δε συμπεριλαμβάνονται στην αναζήτηση του _optimal EDAP_ αφού όσες <<τρέχαν>> το _Cache line size_ τους ήταν τέτοιο που εκτόξευε το EDAP.Επομένως,παρακάτω φαίνονται τα αποτελέσματα για όλες τις άλλες εκτελέσεις του δεύτερου Εργαστηρίου.

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/bzip1.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/bzip2.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/sjeng1.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/sjeng2.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/libm1.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/libm2.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/hmmer1.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/hmmer2.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/mcf1.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/blob/master/Third-lab-gem5-mcpat/Images/mcf2.png" height="420" width="800">

</p>


Από τα παραπάνω διαγράμματα βρήκαμε το _configuration_ με το βέλτιστο **EDAP**. Τα αποτελέσματα παρουσιάζονται στον πίνακα:

|Benchmarks|	Cache Size line	| L1D size  | L1I size |   L2 size   |L1D assoc| L1I assoc|L2 assoc| EDAP | Cost |
|----------|--------------------|-----------|----------|-------------|---------|----------|--------|-----|-----|
|specbzip  |	    64 bytes       |	32 KB    |32 KB	     |	  2 MB          |	   2      |	2  |8	| 75.418499 | 28748.312 |
|spechmmer |	  64 bytes    |16 KB	    |	32 KB     |	 2 MB           |     2       |2	  |8	| 36.773672 | 24906.05 |
|speclibm  |      64 bytes         |	 16 KB   |	32 KB     |	2 MB            |	2          |2       |8	| 298.509671 |	24906.05 |
|specmcf   |	  64 bytes       |16 KB	    | 32 KB	     |	  2 MB          | 2 	          |   2    |	8| 32.1426697311 |	24906.05|
|specsjeng |	  64 bytes          |	 16 KB   |	32 KB     |	 2 MB           | 2	          |  2     |	8| 2531.835016 | 24906.05|

* **Ερώτημα 3**

Όπως παρατηρούμε η συνάρτηση κόστους του προηγούμενου εργαστηρίου προέβλεψε επιτυχώς πώς το **μέγεθος** της μνήμης επηρεάζει σε μεγαλύτερο βαθμό το area απ'οτι το associativity.Επιπλέον, παρατηρούμε πως η αύξηση του _Cache line size_ είναι ο πιο καθαριστικός παράγοντας για το **area** και το **power**,κάτι που δεν αντικατοπτρίζεται στη συνάρτηση κόστους μας.

### Βιβλιογραφία
---

<a name="#cpitype"></a>
[1] https://www.hpl.hp.com/research/mcpat/micro09.pdf  
[2] https://www.hpl.hp.com/research/mcpat/McPATAlpha_TechRep.pdf  
[3] https://en.wikipedia.org/wiki/Performance_per_watt  
[4] https://sites.google.com/site/5md00gem5/assignment


### Κριτική Εργασίας
---

Η τρίτη και τελευταία εργασία κρίνεται απαραίτητη και αρκετά ενδιαφέρουσα ώστε να μπορέσουμε να δούμε συνολικά όλες τις επιπτώσεις μιας συγκεκριμένης αρχιτεκτονικής επεξεργαστή.Η εργασία μας βοήθησε και μας ώθησε να αναζητήσουμε και να κατανοήσουμε πολύ χρήσιμα αντικείμενα και έννοιες.Η εκφώνηση της ήταν κατατοπιστική και πιστεύουμε πως αποκτήσαμε μια πιο διαυγή εποπτεία των  διάφορων trade-off και τις επιπτώσεις που έχουν αυτά στη λειτουργία ενός επεξεργαστή σε όλα τα επίπεδα.

Ωστόσο πρέπει να σημειωθεί πως το εργαλείο **McPAT** δε λειτουργούσε σε πάρα πολλές περιπτώσεις.Το πρόβλημα αυτό δεν απασχόλησε δυστυχώς μόνο εμάς αλλά πολλές ομάδες.
