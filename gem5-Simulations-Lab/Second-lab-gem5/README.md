# Αναφορά δεύτερου εργαστηρίου

## Ομάδα 12   Στασινός Αλκιβιάδης ΑΕΜ 9214 , Τσόμπος Χρήστος AEM 9096

* **Βήμα 1ο:**

* **Ερώτημα 1**   
        

Μας ζητείται να βρούμε τα βασικά χαρακτηριστικά των **_caches_** , δηλαδή τα μεγέθη και τη συσχέτιση (_associativity_) για όλα τα **_Benchmarks_** . Παρατηρούμε ότι δεν δίνεται ρητά κάποιο _flag_ τύπου `--l1_size=` ,επομένως σε όλες τις περιπτώσεις οι κρυφές μνήμες έχουν τις ίδιες _default_ τιμές , όπως φαίνεται από το αρχείο `options.py` . Παραθέτουμε το αντίστοιχο πεδίο:
 
```python

 # Cache Options
    parser.add_option("--external-memory-system", type="string",
                      help="use external ports of this port_type for caches")
    parser.add_option("--tlm-memory", type="string",
                      help="use external port for SystemC TLM cosimulation")
    parser.add_option("--caches", action="store_true")
    parser.add_option("--l2cache", action="store_true")
    parser.add_option("--num-dirs", type="int", default=1)
    parser.add_option("--num-l2caches", type="int", default=1)
    parser.add_option("--num-l3caches", type="int", default=1)
    parser.add_option("--l1d_size", type="string", default="64kB")
    parser.add_option("--l1i_size", type="string", default="32kB")
    parser.add_option("--l2_size", type="string", default="2MB")
    parser.add_option("--l1d_assoc", type="int", default=2)
    parser.add_option("--l1i_assoc", type="int", default=2)
    parser.add_option("--l2_assoc", type="int", default=8)
    parser.add_option("--cacheline_size", type="int", default=64)
``` 
  
Συγκεντρωτικά:

|     Caches    |   Size   |  Associativity| 
| ------------- |:-------------:| :-------: |
|    L1D        | 64 KB         | 2    | 
|L1I  | 32KB     |      2   |          
|  L2  | 2 MB      |    8  |   

* **Ερώτημα 2** 

Ζητείται τώρα να βρεθούν για την εκτέλεση όλων των **_Benchmarks_** τα εξής χαρακτηριστικά: 

i)Χρόνος εκτέλεσης `(sim_seconds)`
ii)_CPI_
iii)Συνολικά _miss rates_ για κάθε τύπο **_caches_**

Αρχικά καναμε λήψη του αρχειου `read_results.sh`,δημιουργήσαμε το κατάλληλο αρχείο `conf.ini` και εκτελώντας την εντολή `$ bash read_results.sh conf.ini` πήραμε τα επιθυμητά αποτελέσματα , τα οποία παραθέτουμε παρακάτω: 

|Benchmarks|	CPI	    | L1D_miss_rate  | L1_miss_rate |   L2_miss_rate   |sim_seconds|
|----------|----------------|----------------|--------------|------------------|-----------|
|specbzip  |	1.683172|	0.014840|	0.000074|	0.281708|	0.084159|
|spechmmer |	1.187362|	0.001645|	0.000205|	0.082246|	0.059368|
|speclibm  |     3.493611|	0.060971|	0.000099|	0.999927|	0.174681|
|specmcf   |	1.109538 |	0.002051|	0.000037|	0.724040|	0.055477|
|specsjeng |	10.270810|	0.121829|	0.000020|	0.999979|	0.513541|

<p align="center">
<img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/Screenshot_1.png"  height="320" width="600">
        
<img src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/l1Imissrate_er1.png"   height="320" width="600">  

<img src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/l1dmissrate_er1.png"   height="320" width="600"> 

<img src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/l2missrate_er1.png"   height="320" width="600"> 

<img src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/simsecond_er1.png"   height="320" width="600">

</p>

Γνωρίζουμε πως το CPI είναι **CPI = 1 + ((L1d.miss_num + L1i.miss_num)\*Ποινή_αστοχίας_L1 + L2.miss_num\*Ποινή_αστοχίας_L2))/Instructions** .  [[1](##cpitype)]
Εδώ κάθε benchmark τρέχει κάθε φορά με τον ίδιο αριθμό instructions(-I 1000000).Αρα μπορούμε να συγκρίνουμε κάθε benchmark άμεσα.

* **Ερώτημα 3**

Στη συνέχεια ακολουθήσαμε την ίδια διαδικασία αλλάζοντας το ρολόι του επεξεργαστή σε **1GHz** , ίδιο με το ρολόι του συστήματος. 
Από το αντίστοιχο αρχείο `stats.txt` μπορούμε να επαληθέυσουμε τις παραπάνω τιμές. 
```
system.clk_domain.clock  1000
system.cpu_clk_domain.clock 1000 
```
Το _clock_ του συστήματος υπάρχει ώστε να επιτυγχάνεται ο συγχρονισμός μεταξύ των διαφόρων μερών που αποτελούν τον υπολογιστή.Η τιμή του καθορίζεται έτσι ώστε ο κάθε κύκλος ρολογιού να επαρκεί για την λειτουργία του πιο αργού μέρους του συστήματος. Όσον αφορά το _clock_ του επεξεργαστή , είναι συνήθως πολλαπλασιασμένο επί έναν αριθμό μεγαλύτερο της μονάδας , ώστε να μπορεί να εκτελεί τις εντολές με μεγαλύτερη ταχύτητα. Όσο μεγαλύτερη η τιμή του , τόσο ταχύτερος είναι ο επεξεργαστής  [[2](##cpitype)]  .Αναζητώντας πληροφορίες στο αρχείο `config.json` παρατηρούμε ότι οι κρυφές μνήμες χρησιμοποιούν το _CPU clock_ . Τα υπόλοιπα στοιχεία ακολουθούν το χρονισμό του συστήματος. Στην περίπτωση προσθήκης 2ου επεξεργαστή , θα χρονίζεται όπως και ο 1ος .



Τα βασικά στατιστικά για την περίπτωση των **1GHz** παρατίθονται στον παρακάτω πίνακα:

|Benchmarks|	CPI	    | L1D_miss_rate  | L1_miss_rate |   L2_miss_rate   |sim_seconds|
|----------|----------------|----------------|--------------|------------------|-----------|
|specbzip  |	1.613367|	0.014683|	0.000074|	0.281702|	0.161337|
|spechmmer |	1.184534|	0.001638|	0.000205|	0.082233|	0.118453|
|speclibm  |     2.623555|	0.060971|	0.000099|	0.999927|	0.262355|
|specmcf   |	1.091249|	0.002051|	0.000037|	0.724040|	0.109125|
|specsjeng |	7.040633|	0.121829|	0.000020|	0.999979|	0.704063|

Όπως ήταν αναμενόμενο , παρατηρώντας τα _sim_seconds_ βλέπουμε πως ο επεξεργαστής με τη μικρότερη συχνότητα είναι πιο αργός.Θα περίμενε κανείς πως εφόσον το _clock_ του επεξεργαστή είχε τη μισή τιμή ,ο χρόνος εκτέλεσης θα ήταν ο διπλάσιος, δηλαδή θα υπήρχε τέλειο **_scaling_** . Είναι προφανές πως κάτι τέτοιο δεν ισχύει. Παραθέτουμε παρακάτω τα **_scaling_** για κάθε _benchmark_ :

|Benchmarks|	Scaling	    | 
|----------|----------------|
|specbzip  |	  1.91          |
|spechmmer |	  1.99         |
|speclibm  |      1.50          |  
|specmcf   |	  1.96          |
|specsjeng |         1.37       |

Φαίνεται ξεκάθαρα από τον πίνακα ότι αν και σε ορισμενα από τα _benchmarks_ προσεγγίζεται ικανοποιητικά το τέλειο _scaling_ , στα **_speclibm_** και **_specsjeng_** είναι ιδιαίτερα χαμηλό . Αυτό οφείλεται στο γεγονός ότι στα συγκεκριμένα _benchmarks_  ο ρυθμός αστοχίας είναι αυξημένος , με αποτέλεσμα να γίνεται αναζήτηση στην κύρια μνήμη **_DRAM_** η οποία λειτουργεί με βάση το χρονισμό του συστήματος και όχι του επεξεργαστή.
        
 * **Βήμα 2ο:**
 ---
 * **Ερώτημα 1**  
        Μας ζητήθηκε στη συνέχεια να μεταβάλλουμε τις παραμέτρους του υποσυστήματος μνήμης με στόχο την αναζήτηση της μέγιστης απόδοσης του επεξεργαστή , η οποία επιτυγχάνεται όταν έχουμε το ελάχιστο **_CPI_** . Οι παράμετροι είναι οι εξής:
 
 ```
- L1 instruction cache size
- L1 instruction cache associativity
- L1 data cache size
- L1 data cache associativity
- L2 cache size
- L2 cache associativity
- Μέγεθος cache line
 ```
 Όπως είναι φανερό ο αριθμός των πιθανών συνδυασμών είναι απαγορευτικά μεγάλος για να εξεταστεί εξ ολοκλήρου.Γι'αυτό ανατρέξαμε στα αποτελέσματα του πρώτου ερωτήματος,στη βιβλιογραφία  [[3](##cpitype)] ,αλλα και σε μελέτη της συμπεριφοράς του κάθε benchmark με σκοπό την εξακρίβωση του βέλτιστου configuration που θα μας δώσει και το ελάχιστο **CPI**.
 
   * **Specsjeng**    
        Αρχικά βλέπουμε πώς το **_specsjeng_** έχει το υψηλότερο CPI ,δηλαδή είναι το πιο αργό απο όλα τα _benchmarks_.Απο τα παραπάνω διαγράμματα μπορούμε να παρατηρήσουμε πως αυτό συμβαίνει λόγω των πολύ υψηλών ποσοστών **L1D και L2 miss rates** σε συνδυασμό με τον μεγάλο αριθμό **data accesses** στις Caches αυτές,συγκεκριμένα `dcache.overall_accesses::total     86382246`
        `l2.overall_accesses::total  5263951 `.Καταλαβαίνουμε λοιπόν πως βασικός άξονας της προσπάθειας μας για ενα χαμηλότερο CPI θα είναι η ελαχιστοποίηση των **miss rates** στις **L1D** και **L2** (Ειδικά στην L2 όπου η ποινή αστοχίας είναι πολυ μεγαλύτερη της L1D).Μέσα απο δοκιμές που διενεργήθηκαν παρατηρήσαμε πως το **sjeng** επηρεάζεται επίσης ιδιαίτερα και απο το **Cache Block Size** , γεγονός που φανερώνει την ανάγκη του για αξιοποιήση του **locality**.
       
   * **Specshmmer**   
        Παρατηρούμε πως το **_hmmer_** είναι το δεύτερο γρηγορότερο benchmark μεχρι στιγμής,με βάση τα αποτελέσματα του πρώτου ερωτήματος,αφου έχει και το δεύτερο χαμηλότερο **CPI**.Ακολουθόντας την ίδια μέθοδο με πρίν ανατρέχουμε αρχικά στο ερώτημα 1,και βλέπουμε πως αν και το _hmmer_ είναι αυτο με το μεγαλύτερο **L1i miss rate** , το miss rate αυτό θεωρείται αρκετά χαμηλό και συνδυάζοντας το με τον επίσης μικρό αριθμό `icache.overall_accesses::total  17142404` , συμπεραίνουμε πως η **L1i** δεν έχει τοσο ουσιαστική επιρροή στο CPI.Επιπλέον,βλέπουμε πως τα accesses στην **L1d** είναι  `dcache.overall_accesses::total  43865006`  και στην **L2** `l2.overall_accesses::total   70496` , ενώ τα **miss rates** είναι αντίστοιχα `dcache.overall_miss_rate::total  0.001645` και `l2.overall_miss_rate::total  0.082246`. Προσέχοντας τα νούμερα αυτά βλέπουμε πώς είναι λογικό για το _hmmer_ να έχει  χαμηλό CPI , αφού τα **accesses** στην _L1d_  είναι σε φυσιολογικά επίπεδα,σχεδόν τα μισά απο το _sjeng_,και τα miss rates σε αυτήν  χαμηλά,ενώ παράλληλα το **L2 miss rate** είναι επίσης χαμηλό και τα  accesses στην **L2** _πολύ_ χαμηλά.Συμπερασματικά,φαίνεται ότι ανάμεσα στις μνήμες τον μεγαλύτερο σχετικό ρόλο στο **CPI** παίζει η **L1d** στην οποία θα προσπαθήσουμε να μειώσουμε τα **misses**.Αυτή θα είναι ο κύριος παράγοντας μαζί με το **Cache Block Size** , με τον οποίο θα ασχοληθούμε για το _optimisation_.

  * **Specsbzip**  
        
       Για το _bzip_ αναζητώντας στο _stats.txt_ τα αντίστοιχα πεδία βλέπουμε  `icache.overall_accesses::total     10198809`                                                                    `dcache.overall_accesses::total 52164486` , `l2.overall_accesses::total  714016` και `icache.overall_miss_rate::total     0.000074` `dcache.overall_miss_rate::total     0.014840` ,`l2.overall_miss_rate::total           0.281708`.Παρατηρούμε το πολύ χαμηλό **miss rate** στην **L1i** αλλά και τον πολύ μικρότερο σε σχέση με την **L1d** _accesses_ που γίνονται στην **L1i** , γεγονός που μας ωθεί να απομακρύνουμε το ενδιαφέρον μας μακριά απο τον παράγοντα της **L1i** , όσον αφορά τη βελτιστοποίηση του CPI.Συγκριτικά με την **L1i**  τα **_accesses_** στην **L1d** είναι  **5** φορές υψηλότερα με μεγάλο σχετικά **miss rate**.Επιπρόσθετα η **L2** μπορεί να έχει χαμηλό αριθμό **προσβάσεων** αλλά το **miss rate** της δεν είναι διόλου αμελητέο.Όλα αυτά καθιστούν την **L1d** ,την **L2** και το **Cache Block Size** βασικούς <<ύποπτους>> για το optimisation.

   * **Speclibm** 
        
        Παραθέτουμε και εδώ τα πεδία `icache.overall_accesses::total      5959253` , `dcache.overall_accesses::total     48806500`,`l2.overall_accesses::total             1488551` και `icache.overall_miss_rate::total     0.000099` , `dcache.overall_miss_rate::total     0.060971` ,`l2.overall_miss_rate::total           0.999927` .Όπως και στα δύο προηγούμενα _benchmarks_ η **L1d** επηρεάζει πιο αισθητά το CPI απο την **L1i** αφού έχει και περισσότερα **accesses** και μεγαλύτερο **miss rate**.Ωστόσο,βασικότερος παράγοντας όλων είναι σίγουρα η **L2** με σημαντικό αριθμό **accesses** και **τεράστιο** (περίπου ίδιο με _sjeng_) **L2 miss rate**. Επομένως και εδώ θα επικεντρωθούμε όπου και στο _bzip_ αλλά _περισσότερο_ στην **L2**.
        
   * **Specmcf**  
        Τελος το _mcf_ , το πιο γρήγορο απο όλα τα _benchmarks_.Για τα **accesses** έχω `icache.overall_accesses::total     27009248`,
`dcache.overall_accesses::total     35173755` , `l2.overall_accesses::total               53631` και για τα **miss rates** `icache.overall_miss_rate::total     0.000037` , `dcache.overall_miss_rate::total     0.002051` , `l2.overall_miss_rate::total           0.724040`.Το _mcf_ φαίνεται να έχει αυξημένο συγκριτικά με άλλα _benchmarks_ αριθμό **accesses** στην **L1i** αλλά το πολύ χαμηλό **miss rate** αυτής θεωρούμαι πως καθιστά τον παράγοντα αυτόν αμελητέο .Έχει σχετικά υψηλό **L2 miss rate** αλλά ταυτόχρονα τα λιγότερα _accesses_ στη μνήμη αυτή.Η **L1d** έχει μικρό αριθμό **miss rate** άλλα πολύ μεγαλύτερο αριθμό accesses απο την **L2**.Και εδώ πιστεύουμαι ότι βασικά χαρακτηριστικά προς βελτίωση αποτελούν η **L1d** και **L2** μαζι με το **Cache Block Size**.Το συγκεκριμένο _benchmark_ είναι ίσως το πιο δύσκολο να βελτιωθεί καθώς το **CPI** του είναι ήδη πολύ κοντά στη μονάδα , οπότε οποιαδήποτε _configuration_   απο εμάς διάφορο του _default_  δε θα επιφέρει δραματικές αλλαγές στην απόδοση.
        
        
        
        
        
   * **Ερώτημα 2**  

Μας ζητήθηκε να βρούμε το **_CPI_** αλλάζοντας κάθε φορά μόνο μία από τις παραπάνω παραμέτρους. Με τον τρόπο αυτό είναι εφικτό να γίνει μια καλή εκτίμηση σχετικά με το πως η κάθε μια από αυτές επιδρά στην απόδοση .Κατασκευάσαμε αρχικά το **Bash script** `makeall.sh` το οποίο παίρνει κάθε benchmark εκτελεί όλους τους δυνατούς συνδυασμούς μεταβάλλοντας κάθε φορά **μόνο** μία παράμετρο.Στη συνέχεια το ίδιο φτιάχνει τα κατάλληλα config files τα οποία και περνά στο _read_results.sh_ . Τα αποτελέσματα που πήραμε παρουσιάζονται αναλυτικά με τα  παρακάτω διαγράμματα και σχολιάζονται με βάση τις γνώσεις που αντλήσαμε απο τη βιβλιογραφία [[3](##cpitype)].
 
 <p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/mastergem5-Simulations-Lab//Second-lab-gem5/images/cachesize_line.png" height="320" width="600">

</p>

Παρατηρούμε ότι με αύξηση του **Cache line size** αρχικά τα **_CPI_** μειώνονται , με τα **_specsjeng_** και **_speclibm_** να επηρεάζονται σε μεγαλύτερο βαθμό. Αυτό οφείλεται στο γεγονός ότι με αξιοποιείται η χωρική τοπικότητα με αποτέλεσμα να μειώνεται αισθητά ο ρυθμός των αστοχιών. Ωστόσο , μεγαλύτερη αύξηση συνεπάγεται υψηλότερη ποινή αστοχίας , κάτι που επιδρά αρνητικά στην τιμή του _CPI_ για ορισμένα **_benchmarks_** μετά απο κάποιο σημείο . 

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/L1D_size.png" height="320" width="600">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/L1I_size.png" height="320" width="600">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/L2_size.png" height="320" width="600">

</p>

Όσον αφορά την επίδραση του μεγέθους των κρυφών μνημών , δεν υπάρχει κάποια σημαντική μεταβολή στην απόδοση. Πιο έντονη επίδραση παρατηρείται από την αλλαγή του _L1 instruction cache size_ στο **_specmcf_** . Το μεγαλύτερο μέγεθος μιας κρυφής μνήμης ελλατώνει το ρυθμό αστοχίας , ενω παράλληλα αυξάνει και το χρόνο ευστοχίας , γεγονός που δικαιολογεί τις πολύ μικρές αυξομοιώσεις του _CPI_ .

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/L1D_assoc.png" height="320" width="600">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/L1I_assoc.png" height="320" width="600">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/L2_assoc.png" height="320" width="600">

</p>

Τέλος παρατηρούμε οτι και η επίδραση της συσχετικότητας είναι μικρή . Ακριβώς όπως και στην περίπτωση των μεγεθών της κρυφής μνήμης , υψηλότερη συχετικότητα περιορίζει τις αστοχίες βελτιώνοντας την απόδοση , προκαλεί όμως και αύξηση του χρόνου ευστοχίας . Για το λόγο αυτό όπως φαίνεται και από τα διαγράμματα μετά από κάποιο σημείο δεν υπάρχει βελτίωση της απόδοσης για ορισμένα **_benchmarks_** , ενώ για άλλα βλέπουμε μεγαλύτερο _CPI_ .

Βασιζόμενοι στις αρχικές αυτές δοκιμές , τρέξαμε πληθώρα κατάλληλα διαμορφωμένων διαφορετικών εκτελέσεων και βρήκαμε τις βέλτιστες παραμέτρους για κάθε **_benchmark_**  ,οι οποίες παρατίθονται στον παρακάτω πίνακα. 
 
|Benchmarks|	Cache Size line	| L1D size  | L1I size |   L2 size   |L1D assoc| L1I assoc|L2 assoc| CPI |
|----------|--------------------|-----------|----------|-------------|---------|----------|--------|-----|
|specbzip  |	    256 bytes       |	128 KB    |32KB	     |	  4 MB          |	   256       |	4  |4	| 1.585044 |
|spechmmer |	  256 bytes    |128 KB	    |	128 KB     |	 1 MB           |     64        |16	  |8	| 1.179201 |
|speclibm  |      2048 bytes         |	 128 KB   |	128 KB     |	4 MB            |	2          |4       |1	| 1.499254|	
|specmcf   |	  128 bytes       |128 KB	    | 32 KB	     |	  4 MB          | 16 	          |   4    |	8| 1.084003 |	
|specsjeng |	  2048 bytes          |	 128 KB   |	128 KB     |	 4 MB           | 16	          |  64     |	256| 3.071813 |	

**Σημαντική σημείωση** : Οι παράμετροι που φαίνονται παραπάνω πετυχαίνουν  το καλύτερο **CPI** και μάλιστα με τον πιο οικονομικό τρόπο,εφόσον παρατηρήσαμε πώς περαιτέρω αύξηση κάποιας δίνει ακριβως το ίδιο CPI ή χειρότερο.Επιπλέον για τις ανάγκες του ερωτήματος αυτού δημιουργήθηκαν 4 **Bash scripts** (`optimaljeng.sh`,`optimalbzip.sh`,`optimallibm.sh`,`optimalhmmer_mcf.sh`) τα οποία εκτελούσαν όλες τις δοκιμές, έφτιαχναν τα κατάλληλα **conf.ini** αρχεία και εκτελούσαν το **read_results.sh** για αυτό.  

* **Βήμα 3ο**
---

Στο βήμα αυτό μας ζητείται αρχικά να ορίσουμε μία **συνάρτηση κόστους** υλικού για τα διάφορα πιθανά _configurations_ που μπορεί να έχει μία ιεραρχία **Cache**.
Θεωρούμε αναγκαίο πρώτα απο όλα να ορίσουμε τί και πώς κοστίζει σε μία μνήμη _Cache_.  [[3](##cpitype)]

* **Ποσότητα υλικού και μέγεθος μνήμης**  
        Ο **κύριος** παράγοντας που προσθέτει κόστος είναι φυσικά η **ποσότητα του υλικού**.Δηλαδή για παράδειγμα μία μνήμη **L1 64KB** προφανώς κοστίζει **2** φορές όσο μία **32KB**.

* **Συσχετικότητα**  
        Οι **_direct-mapped_** μνήμες Cache είναι πιο φθηνές απο μνήμες με **2-way associativity** και το κόστος ανεβαίνει όσο αυξάνεται η συσχετικότητα.Ο λόγος που συμβαίνει αυτό είναι ότι η υλοποίηση μνήμης με συσχέτιση προσθέτει πολυπλοκότητα και  περισσότερο υλικό στη κατασκευή.

Ανατρέχοντας στη βιβλιογραφία [[4](##cpitype)]  προσπαθήσαμε να βρούμε το σχετικό κόστος μεταξύ μίας **L1** και μίας **L2** μνήμης Cache,έτσι ώστε να επιλέξουμε τους σωστούς συντελεστές για τη συνάρτηση μας.Είναι γνωστό πώς η **L1** είναι πολύ πιο γρήγορη απο την **L2**.Για να συμβαίνει αυτό , η L1 τοποθετείται όσο πιο κοντά είναι δυνατό στον επεξεργαστή με στόχο τη μέγιστη μείωση των καθυστερύσεων.Επιπλέον το υλικό της είναι _optimised_ για ταχύτητα και πιο περίπλοκο καθώς μπορεί να κάνει _access_ στα _tags_ και στο _data_ ταυτόχρονα ,αντίθετα το υλικό της **L2** στοχεύει λιγο περισσότερο στη χωρητικότητα παρά στη ταχύτητα και  εκτελεί τα παραπάνω _accesses_ σειριακά.Καταλαβαίνουμε λοιπόν πως μια **L1** είναι **σαφώς** πιο ακριβή απο μία **L2**.

Κάνοντας εκτενή αναζήτηση [[5](##cpitype)] (σελίδα 6) βρήκαμε ότι το κόστος μιας μνήμης τεχνολογίας **SRAM** είναι περίπου 100 φορές μεγαλύτερο από αυτό της **DRAM** . 

Στη συνέχεια βρήκαμε τα _response latencies_ για την κύρια μνήμη , καθώς και για τις _L1_ και _L2_ για διάφορα _benchmarks_ που έτρεξαν σε πραγματικούς υπολογιστές. Ορισμένα από τα αποτελέσματα φαίνονται στις παρακάτω εικόνες:



Example 1           |  Example 2
:-------------------------:|:-------------------------:
![](https://www.techpowerup.com/forums/attachments/3000-16-18-18-36-png.130436/)  |  ![](https://cdn.batutin.net/blog/2015/2015-02-06-aida64-extreme/ram_cache_benchmark.jpg)

 Το _response latency_ για τη **DRAM** κυμαίνεται στα **75-100 ns** , για την **L1** υπολογίζεται περίπου στο **1 ns** ενώ τέλος,για την **L2** παίρνει τιμές από **3 ns** μέχρι  **5 ns**. Εφόσον η _L1_ είναι κατά 3 με 5 φορές ταχύτερη της _L2_ και λαμβάνοντας υπόψιν όλα τα παραπάνω θεωρήσαμε πως μια καλή αναλογία ως προς το κόστος είναι **12:1** . Πιστεύοντας παράλληλα πως η αύξηση της συσχετικότητας απαιτεί αρκετά χαμηλότερο κόστος αποφασίσαμε να πολλαπλασιάσουμε τη ρίζα αυτής με το μέγεθος της κάθε αντίστοιχης μνήμης _cache_ διαιρόντας δια 10,ετσι ώστε για παράδειγμα μια μνήμη **L2**  με _asscociativity_ **256** να μη καταλήγει με συντελεστή κόστους μεγαλύτερο απο αυτόν που της προσδίδεται λόγω του μεγέθους της ( * 10 ). **Βασικό κριτήριο** στην επιλογή της συνάρτησης κόστους αποτέλεσε επίσης το ότι αν **διπλασιαστεί η ποσότητα του υλικού** , τότε **διπλασιάζεται και το κόστος** (γι'αυτό και κάθε όρος πολλαπλασιάζεται με τον αντίστοιχο όρο μεγέθους)Επομένως η συνάρτηση κόστους θα είναι η εξής:
 

   
<p align="center">
        <img  src="https://latex2image.joeraut.com/output/img-807af5179f5c27ce.jpg">

</p>
 

 
 Στη συνέχεια σχηματίζουμε μια **συνάρτηση συσχέτισης** ,η οποία θα λαμβάνει υπόψη της και τη παραπάνω συνάρτηση αλλά και **την αύξηση της απόδοσης** για δεδομένο _configuration_.Για τη συνάρτηση συσχέτισης θα χρησιμοποιήσουμε ως σημείο αναφοράς το _CPI_ ενός επεξεργαστή με παραμέτρους που παίρνουν τις **ελάχιστες δυνατές τιμές**.Καταλήξαμε στην:
 
<p align="center">
        <img  src="https://latex2image.joeraut.com/output/img-b6e3bdd2c699a98d.png">

</p>

Όπου   
<img src="https://latex2image.joeraut.com/output/img-c4652663244ac3a3.jpg">   
                                                                                
και **CPInow** το CPI με το εκάστοτε _configration_ και **CPIcheapest** το CPI όταν έχουμε το οσο πιο δυνατόν φθηνό _configuration_.Η εξίσωση πολλαπλασιάζεται με το _gain_  εις τη **10η** ώστε να επιτύχουμε μία καλή ισορροπία μεταξύ κόστους και επιτάχυνσης (δίνουμε λίγο μεγαλύτερο βάρος στην αύξηση της απόδοσης παρά στο κόστος).
  
Επομένως ψάχνουμε το _configuration_ που θα μας δώσει χαμηλότερη τιμή στη παραπάνω συνάρτηση.

Πρώτα απο όλα,θεωρούμαι πως το **optimal** (_του βήματος 2_) configuration κάθε benchmark όπως ακριβώς φαίνεται στον πίνακα του βήματος 2 είναι και αυτό με το **μέγιστο κόστος**.Ξεκινόντας απο αυτό θα προσπαθήσουμε να περιορίζουμε κάθε φορά τις παραμέτρους του και να κάνουμε μετρήσεις ώστε να εντοπίσουμε την "χρυσή τομή" κόστους-απόδοσης.

Γνωρίζουμε σύμφωνα με τα παραπάνω πως μεγαλύτερη συμβολή στην αύξηση του κόστους έχουν τα μεγέθη των μνημών,γι'αυτό και αρχικά βρίσκουμε το καλύτερο **_cost-performance_** configuration ,εκτελόντας **κάθε** διαφορετικό συνδυασμό **μεγέθους μνήμης** ( απο αυτούς που προκύπτουν σύμφωνα με την ακριβώς παραπάνω θεώρηση) ,κρατόντας τις υπόλοιπες παραμέτρους σταθερέ και υπολογίζοντας κάθε φορά την τιμή της συνάρτησης συσχέτισης( οσο μικρότερη τόσο καλύτερα).Στη συνέχεια ,αφού το βρούμε,μεταβάλλουμαι σε αυτό με διάφορους συνδυασμούς τα **associastivities** και φτάνουμε εν τέλει στη βέλτιστη λύση. 

**Σημείωση** : Για τις ανάγκες του ερωτήματος αυτού δημιουργήθηκαν τρία **_python scripts_** ( 1 για το bzip με όνομα `scorebzip.py`, 1 για το sjeng `scorejeng.py`  και 1 για τα υπόλοιπα `scorerest.py`).Τα _scripts_ αυτά ,τα οποία υπάρχουν στον αντίστοχο φάκελο αυτού του repository, κάνουν πράξη τον παραπάνω συλλογισμό και υπολογίζουν για κάθε εκτέλεση την τιμή που έχει αυτή στη συνάρτηση συσχέτισης.Στο τέλος υπολογίζει για κάθε _benchmark_ ποιά εκτέλεση έδωσε την **ελάχιστη** τιμή.Για την εκτύπωση των αποτελεσμάτων με τη χρήση του read_results.sh χρησιμοποιείται το ίδιο _for loop_ που υπάρχει και στο τέλος των **bash script** (πχ makeall.sh) απλά δινοντάς του τα κατάλληλα _paths_.

Ακολουθούν δυο διαγράμματα για κάθε διαφορετικό **Benchmark**.Το πρώτο παρουσιάζει το πλήθος των δοκιμών που εκτελέσαμε με X άξονα το **CPI** και Y το κόστος,ενώ το δεύτερο τις τιμές της συνάρτησης συσχέτισης.**Η χρυσή τομή** έγκειται στο σημείο όπου η συνάρτηση συσχέτισης παρουσιάζει **Ολικό ελάχιστο**.



 <p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/bzipvscost.jpg" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/bzipcosimg.jpg" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/sjeng1.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/sjeng2.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/hmmer1.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/hmmer2.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/gem5-Simulations-Lab/Second-lab-gem5/images/libm1.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/blob/master/Second-lab-gem5/images/libm2.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/mcf1.png" height="420" width="800">

</p>

<p align="center">
        <img  src="https://github.com/astasinos/University-Projects/blob/master/gem5-Simulations-Lab/Second-lab-gem5/images/mcf2.png" height="420" width="800">

</p>

Από τα παραπάνω διαγράμματα βρήκαμε το βέλτιστο συνδυασμό μεταξύ απόδοσης του επεξεργαστή και κόστους. Τα αποτελέσματα παρουσιάζονται στον πίνακα:

|Benchmarks|	Cache Size line	| L1D size  | L1I size |   L2 size   |L1D assoc| L1I assoc|L2 assoc| CPI |
|----------|--------------------|-----------|----------|-------------|---------|----------|--------|-----|
|specbzip  |	    256 bytes       |	32 KB    |16KB	     |	  1 MB          |	   128       |	4  |4	| 1.691034 |
|spechmmer |	  256 bytes    |16 KB	    |	16 KB     |	 1 MB           |     8       |8	  |1	| 1.18233 |
|speclibm  |      2048 bytes         |	 16 KB   |	16 KB     |	1 MB            |	2          |4       |1	| 1.508334|	
|specmcf   |	  128 bytes       |16 KB	    | 32 KB	     |	  1 MB          | 16 	          |   4    |	8| 1.091407 |	
|specsjeng |	  2048 bytes          |	 16 KB   |	16 KB     |	 1 MB           | 8	          |  2     |	2| 3.0771 |

Όπως ήταν αναμενόμενο οι τιμές των **CPI** παρουσίασαν μια μικρή αύξηση η οποία όμως δεν αποτελεί πρόβλημα λαμβάνοντας υπόψιν το οικονομικό κόστος που γλιτώνουμε .

### Βιβλιογραφία
---
<a name="#cpitype"></a>
[1] https://personal.utdallas.edu/~gxm112130/EE6304FA17/project1.pdf  
[2] https://www.webopedia.com/TERM/C/clock_speed.html  
[3] https://www3.nd.edu/~mniemier/teaching/2011_B_Fall/lectures/19_PPT_1up.pdf  
[4] https://stackoverflow.com/questions/4666728/why-is-the-size-of-l1-cache-smaller-than-that-of-the-l2-cache-in-most-of-the-pro/49863933#49863933  
[5]https://cs.nyu.edu/courses/fall13/CSCI-UA.0201-003/lecture17-18.pdf?fbclid=IwAR2gTPJ-TO9gKWrMpe3UXyAIq9U6VgEocB-d1g--hKLI8Gxq_34soagb0aU
 ### Κριτική Εργασίας
---
Η δεύτερη εργασία ήταν ιδιαίτερα βοηθητική καθώς απαιτούσε εκτενή αναζήτηση στη βιβλιογραφία με αποτέλεσμα την εμβάθυνση στη λειτουργία του επεξεργαστή και των τμημάτων που τον αποτελούν, αλλά και στο ρόλο της κρυφής μνήμης στην απόδοσή του.Αξίζει να σημειωθεί πως είχε καθοριστικό ρόλο στην καλύτερη κατανόηση του gem5 και των δυνατοτήτων του . Παράλληλα αποτελούσε μια ικανοποιητική προσέγγιση στην πραγματική εργασία που καλείται να εκτελέσει ένας  αρχιτέκτονας υπολογιστών , εκτελώντας **benchmarks** για διαφορετικές παραμέτρους αναζητώντας τη βέλτιστη λύση. Όσον αφορά την εκφώνηση , ήταν ξεκάθαρη και κατατοπιστική ,με μια μικρή εξαίρεση στο ερώτημα που αφορά τη δημιουργία της συνάρτησης κόστους.Τέλος , πιστεύουμε πως ίσως θα έπρεπε ο αριθμός των **benchmarks** να ήταν λίγο μικρότερος ώστε να ελλατωθεί ο "νεκρός" χρόνος που χρειάστηκαν για να τρέξουν για όλους τους συνδυασμούς.
Συνολικά μπορούμε να πούμε πως η εργασία αυτή δημιουργεί μια πιο πλήρη και ρεαλιστική εικόνα της αρχιτεκτονικής υπολογιστών.
