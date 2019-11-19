# Αναφορά πρώτου εργαστηρίου   

Σκοπός της συγκεκριμένης αναφοράς είναι η παρουσίαση των αποτελεσμάτων τών προσομοιώσεων και η απάντηση των ερωτημάτων , που ζητούνται απο το πρώτο εργαστήριο,στα πλαίσια του μαθήματος _Αρχιτεκτονική Προηγμένων Υπολογιστών_.


 #### Ερώτημα 1
---
  Αρχικά τρέχοντας την εντολη ```./build/ARM/gem5.opt -d hello_result configs/example/arm/starter_se.py
--cpu="minor" "tests/test-progs/hello/bin/arm/linux/hello"```
  
  εξομειώνουμε το προυπάρχων παραδειγματικό πρόγραμμα ```tests/test-progs/hello/bin/arm/linux/hello``` του gem5 με τύπο επεξεργαστή _"minor"_  γράφοντας τα αποτελέσματα στο φάκελο ```hello_result``` και χρησιμοποιόντας ως _config file_ το αρχείο **starter_se.py**.
  
  Στο πρώτο ερώτημα μας ζητείται να ανοίξουμε το αρχείο αυτό(_starter_se.py_) και να καταγράψουμε τις βασικές παραμέτρους της       εξομοίωσης καθώς και τα βασικά χαρακτηριστικά του συστήματος προς εξομοίωση.
  
  Ανοίγοντας το αρχείο και ανατρέχοντας στη _main function_ μπορούμε να παρατηρήσουμε τα εξής:
   ```python
    def main():
    parser = argparse.ArgumentParser(epilog=__doc__)

    parser.add_argument("commands_to_run", metavar="command(s)", nargs='*',
                        help="Command(s) to run")
    parser.add_argument("--cpu", type=str, choices=cpu_types.keys(),
                        default="atomic",
                        help="CPU model to use")
    parser.add_argument("--cpu-freq", type=str, default="4GHz")
    parser.add_argument("--num-cores", type=int, default=1,
                        help="Number of CPU cores")
    parser.add_argument("--mem-type", default="DDR3_1600_8x8",
                        choices=ObjectList.mem_list.get_names(),
                        help = "type of memory to use")
    parser.add_argument("--mem-channels", type=int, default=2,
                        help = "number of memory channels")
    parser.add_argument("--mem-ranks", type=int, default=None,
                        help = "number of memory ranks per channel")
    parser.add_argument("--mem-size", action="store", type=str,
                        default="2GB",
                        help="Specify the physical memory size")

    args = parser.parse_args()
    ...
   ```
   Όπως φαίνεται η _main_ χρησιμποιεί το module **_parser_** με σκοπό να ορίσει κάποιες βασικές παραμέτρους της εξομοίωσης όπως ο χρήστης τις δίνει κατά τη κλήση του _gem5_  απο το _shell_.Σε διαφορετική περίπτωση,αν δηλαδή ο χρήστης δε δώσει τιμή για κάποια απο τις δυνατές παραμέτρους,τότε η _main_ ορίζει μόνη της **_default_** τιμές για αυτές.Για παράδειγμα,αφού κατα την κλήση δεν ορίσαμε τη συχνότητα του επεξεργαστή θα χρησιμοποιηθεί η _default_ τιμή **4GHz** .  
   Πιό Συγκεκριμένα στη δική μας εξομοίωση ορίσαμε **μόνο** την παράμετρο **_--cpu="minor"_**  ,οπότε όλες οι άλλες παράμετροι παίρνουν τις _default_ τιμές.  
  
  
    
   Λίγο πιο κάτω στη `main()`  ορίζεται `root.system = create(args)` , όπου και καλείται η συνάρτηση `create()` με τις παραμέτρους μας.Μέσα στην `create()` παρατηρούμε την εντολη `system = SimpleSeSystem(args)` η οποία καλεί τον _constructor_ της κλάσης  `SimpleSeSystem`. Μέσα σε αυτόν βλέπουμε αρχικά τήν εντολή `cache_line_size = 64` η οποία δηλώνει ότι το μέγεθος ενός **_block_** της _cache_ είναι **64 Bytes** .
   Ύστερα παρατηρούμε τις εντολές
   
```python
   self.voltage_domain = VoltageDomain(voltage="3.3V")
   self.clk_domain = SrcClockDomain(clock="1GHz",
                                         voltage_domain=self.voltage_domain)
```
Οι οποίες ορίζουν την τάση λειτουργίας του συστήματος στα **3.3V** και το **clock** στα  **1GHz**.

Όλα τα χαρακτηριστικά του συστήματος είναι μέχρι στιγμής ξεκάθαρα εκτός απο την μνήμη **_Cache_**.

  #### Εξακρίβωση της cache
  
  
  
Συνεχίζοντας στον ίδιο _constructor_ βλέπουμε την εντολή
```python
 self.cpu_cluster = devices.CpuCluster(self,
                                              args.num_cores,
                                              args.cpu_freq, "1.2V",
                                              *cpu_types[args.cpu])
```
Ουσιαστικά καλείται ο  _constructor_ της κλάσης `CpuCluster`, που ανήκει στο αρχείο *_devices.py_* και προσθέτει τους επεξεργαστές στο σύστημα μας.Τον ορισμό του _constructor_ της κλάσης αυτής μπορούμε να τον βρούμε στο _devices.py_ και είναι 
```python
def __init__(self, system,  num_cpus, cpu_clock, cpu_voltage,
                 cpu_type, l1i_type, l1d_type, wcache_type, l2_type)
```

Το `args.num_cores` δηλώνει τον αριθμό των επεξεργαστών(στη δική μας περίπτωση **ένας**),το `args.cpu_freq` ορίζει τη συχνότητα λειτουργίας αυτών(εμάς είναι _default_ στα **4GHz**),το `"1.2V"` την τάση λειτουργίας του CPU, και τέλος το `*cpu_types[args.cpu]` κάνει _unpack_ τα _items_ του αντίστοιχου επεξεργαστή απο το _dictionary_  _cpu_types_  (στη περίπτωση μας  **"minor"** ).  

  
```python
cpu_types = {                 # cpu types dictionary in starter_se.py
    "atomic" : ( AtomicSimpleCPU, None, None, None, None),
    "minor" : (MinorCPU,
               devices.L1I, devices.L1D,
               devices.WalkCache,
               devices.L2),
    "hpi" : ( HPI.HPI,
              HPI.HPI_ICache, HPI.HPI_DCache,
              HPI.HPI_WalkCache,
              HPI.HPI_L2)
}
```
Επομένως η παράμετρος `l1i_type` αντιστοιχίζεται με το `devices.L1I` , η `l1d_type` με `devices.L1D` κ.ο.κ.
Ανατρέχοντας στο αρχείο **_devices.py_** και μάλιστα στην κλάση `python L1I` βλέπουμε στο περιεχόμενο της:

```python
class L1D(L1_DCache):
    ...
    size = '32kB'
    assoc = 2
    response_latency = 1 
```

Τελικά  στο  **starter_se.py** υπάρχουν οι εντολές
```python
if self.cpu_cluster.memoryMode() == "timing":
            self.cpu_cluster.addL1()
            self.cpu_cluster.addL2(self.cpu_cluster.clk_domain)
```

 Το _if_ statement εδώ ελέγχει αν ο επεξεργαστής μας είναι σε **_timing_** memory mode , και αφού έχουμε επιλέξει **minor** cpu type **αυτό ισχύει**.Έτσι,καλούνται οι συναρτήσεις `addL1()` και `addL2()` για την προσθήκη **_L1_** και **_L2_** **cache** στο σύστημα μας , με βάση τις κλάσεις `L1D`,`L1I` κ.ο.κ.

**Άρα τελικά η L1I Cache ορίζεται με μέγεθος 32ΚΒ , associativity=2 και 16 write buffers**,ενώ το **_cache line size_** όπως ορίζεται _by default_ στο _starter.py_  είναι **64 Bytes** .Με τον ίδιο τρόπο βρίσκουμε τα αντίστοιχα στοιχεία για κάθε _cache_.

<h3 align="center">Συγκεντρωτικά</h3> 

  * Για το σύστημα
      1. Clock - **1GHz**
      2. Voltage - **3.3V**
      
  * Για τον επεξεργαστή
      1. Clock - **4GHz**
      2. Type - **"Minor"**
      3. Number of cores - **1**
      4. Mode - **timing mode**
      5. Voltage - **1.2V**
   
   * Για την _Cache_ 
   
       |     Caches    |   Size   |  Associativity| Response Latency |
       | ------------- |:-------------:| :-------: |:---------:|
       |    L1D        | 32 KB         | 2    | 1 ns |
       |L1I  | 48 KB     |      3    |          1 ns |
       |  L2  | 1 MB      |    16  |  5 ns | 
        

   * Για τη _DRAM_
   
       1. Size - **2 GB**
       2. Type - **DDR3_1600_8x8** (Clock - 1600 MHz)
       3. Channels - **2**
       
       
       #### Ερώτημα 2
---
Ζητείται να επαληθεύσουε τα βασικά χαρακτηριστικά του 1ου ερωτήματος εξετάζοντας τα αρχεία **_config.ini_** και **_confing.json_**.

* Για το σύστημα :

```
i.[system.clk_domain]
clock=1000 

```
Από το _clock_ το οποίο είναι 1000 picoseconds προκύπτει πως η συχνότητα του επεξεργαστή είναι **_1GHz_**

```
ii.[system.voltage_domain]
voltage=3.3 V
```
* Για τον επεξεργαστή :

```
i.[system.cpu_cluster.clk_domain]
clock=250

```

Από το _clock_ το οποίο είναι 250 picoseconds προκύπτει πως η συχνότητα του επεξεργαστή είναι **_4GHz_**
```
ii.[system.cpu_cluster.cpus]
type=MinorCPU
```

```
iii.[system.cpu_cluster.cpus]
numThreads=1
``` 
Συμβολίζει τον αριθμό των πυρήνων ,  που στην περίπτωσή μας είναι 1.

```
iv.[system]
mem_mode=timing
```
```
v.[system.cpu_cluster.voltage_domain]
voltage=1.2 V
```
* Για την **_Cache_**:

 **_L1D_** :
 
 ```
 [system.cpu_cluster.cpus.dcache]
 i.size=32768
 ii.assoc=2
 iii.write_buffers=16
 ```
 Διαιρώντας το **32768** με το 1024 προκύπτει ότι η **_L1D_** είναι **_32KB_** .

**_L1D_** :

```
[system.cpu_cluster.cpus.icache]
i.size=49152
ii.assoc=3
iii.write_buffers=8
```
Διαιρώντας το **49152** με το 1024 προκύπτει ότι η **_L1I_** είναι **_48KB_** .

**_L2_** :

```
[system.cpu_cluster.l2]
i.size=1048576
ii.assoc=16
iii.write_buffers=8
```
Διαιρώντας το **1048576** με το (1024) προκύπτει ότι η **_L2_** είναι **_1MB_** 


* Για την **_DRAM_** :

```
i.[system]
mem_ranges=0:2147483647
```
Διαιρώντας την τιμή **2147483647** με το (1024)^3 προκύπτει το μέγεθος της **_DRAM_** : **_2GB_** .

```
ii.Ο τύπος δεν αναγράφεται στο αρχείο.
```
    


#### Ερώτημα 3
---

* **Υποερώτημα A**

Αρχικά παρατίθεται μια σύντομη περιγραφή των διάφορων μοντέλων **In Order CPUs** που υλοποιούνται στον _gem5_,βασισμένη κυρίως σε πληροφορίες που αντλήσαμε απο το _ARM University_.[[1](##1)] Συγκεκριμένα παρουσιάζονται τα μοντέλα **AtomicSimpleCPU**,**TimingSimpleCPU** και **MinorCPU**.

![CPUs](https://github.com/astasinos/First-lab-gem5/blob/master/images/cpus.png)

Ο _AtomicSimpleCPU_ μαζί με τον _TimingSimpleCPU_ είναι βασισμένοι στον τύπο _BaseSimpleCPU_ όπως φαίνεται και στην παραπάνω εικόνα.Τα μοντέλα αυτά χρησιμοποιούνται κυρίως για απλοϊκές προσομοιώσεις παραλείποντας πολλές λεπτομέρειες,όταν θέλουμε να φτάσουμε άμεσα και γρήγορα σε συγκεκριμένο κομμάτι της προσομοίωσης που μας ενδιαφέρει περισσότερο.

* AtomicSimpleCPU  
    Ο _Atomic_ αξιοποιεί  [_Αtomic memory access_](#parartima).Στον _gem5_ το μοντέλο του _Atomic_  πραγματοποιεί σε κάθε _tick()_ του CPU όλες τις απαιτούμενες διεργασίες για μία εντολή.Ο _AtomicSimpleCPU_ δε διαθέτει _pipelining_,δηλαδή κάθε δεδομένη στιγμή εκτελείται **μία** μόνο εντολή.Η χρήση του _Atomic_  μοντέλου συνήθως προσφέρει πολυ ταχείες συγκριτικά προσομοιώσεις.

* TimingSimpleCPU  
    Σε αντίθεση με τον _AtomicSimpleCPU_ , το μοντέλο αυτό υιοθετεί το [_Timing memory mode_](#parartima). Αυτό συνεπάγεται πως περιμένει τον τερματισμό κάθε φορά του _memory access_ προτού συνεχίσει,γι'αυτό προσφέρει και μια πιο λειτουργική προσέγγιση "χρονομέτρησης".Ο _TimingSimpleCPU_ όπως και ο _Atomic_ **δε** διαθέτει _pipelining_. Οι αριθμητικές εντολές χρειάζονται μόλις ένα κύκλο επεξεργασίας ,ενώ οι προσβάσεις στη μνήμη συνήθως πολλαπλούς.  
 
 * MinorCPU  
     Στη προσπάθεια για εξομοίωση όσο το δυνατόν **_πραγματικά_** συστήματα ,πρέπει να χρησιμοποιήσουμε πιο λεπτομερή και πολύπλοκα μοντέλα επεξεργαστή.Τα μοντέλα αυτα τρέχουν πιο αργά απο τα παραπάνω _simplified_ μοντέλα.Ένας τέτοιος επεξεργαστής είναι ο **_MinorCPU_** , διάδοχος του παλαιότερου _InorderCPU_.Σε αντίθεση με τα 2 μοντέλα **_CPU_** που προαναφέρθηκαν ο **_MinorCPU_** διαθέτει _pipelining_ τεσσάρων σταδίων,κάτι που συμβάλλει σημαντικά στην ταχύτητα του επεξεργαστή καθώς επιτρέπει την εκτέλεση μέχρι και 4 εντολών ταυτόχρονα.Τα 4 αυτά στάδια αποτελούν 2 στάδια προσκόμισης **_(fetch)_** εντολών , ένα στάδιο αποκωδικοποίησης και τέλος το στάδιο εκτέλεσης των εντολών.
     
     * **_Fetch1_** :
     Στο πρώτο βήμα προσκομίζονται οι απαραίτητες εντολές από την **_cache_** και στη συνέχεια στέλνονται για χρήση στο δεύτερο βήμα. 
     
     * **_Fetch2_** :Στο βήμα αυτό το σύνολο των δεδομένων που ελάβε από το **_Fetch1_** διαχωρίζεται με τη χρήση ενός **_input buffer_** σε ατομικές εντολές οι οποίες πακετάρονται και στέλνονται προς αποκωδικοποίηση.
     
     * **_Decode_** :Οι πακεταρισμένες εντολές του **_Fetch2_** αποσυντίθονται σε εντολές χαμηλού επιπέδου **_(micro-ops)_**, πακετάρονται ξανά και στέλνονται στο τελικό στάδιο για εκτέλεση.
     
     * **_Execute_** :Στο τελεύταιο στάδιο εκτελούνται οι εντολές καθώς επίσης και οι λειτουργίες της μνήμης , κλείνοντας έτσι τον κύκλο της διοχέτευσης.
     
     
     
* **Υποερώτημα B**.


Μας ζητήθηκε να εκτελέσουμε ένα πρόγραμμα σε γλώσσα _C_ για 2 τύπους επεξεργαστών και να παραθέσουμε τους χρόνους εκτέλεσης χρησιμοποιώντας το αρχείο **_stats.txt_** για κάθε περίπτωση.

Αποτελέσματα για **_MinorCPU_**:
```
---------- Begin Simulation Statistics ----------
final_tick                                   33428000                       # Number of ticks from beginning of simulation (restored from checkpoints and never reset)
host_inst_rate                                  19489                       # Simulator instruction rate (inst/s)
host_mem_usage                                 716564                       # Number of bytes of host memory used
host_op_rate                                    22200                       # Simulator op (including micro ops) rate (op/s)
host_seconds                                     0.44                       # Real time elapsed on the host
host_tick_rate                               76212567                       # Simulator tick rate (ticks/s)
sim_freq                                 1000000000000                       # Frequency of simulated ticks
sim_insts                                        8546                       # Number of instructions simulated
sim_ops                                          9737                       # Number of ops (including micro ops) simulated
sim_seconds                                  0.000033                       # Number of seconds simulated
sim_ticks                                    33428000        
```

Αποτελέσματα για **_TimingSimpleCPU_**:
```
---------- Begin Simulation Statistics ----------
final_tick                                   37921000                       # Number of ticks from beginning of simulation (restored from checkpoints and never reset)
host_inst_rate                                 338871                       # Simulator instruction rate (inst/s)
host_mem_usage                                 711956                       # Number of bytes of host memory used
host_op_rate                                   383062                       # Simulator op (including micro ops) rate (op/s)
host_seconds                                     0.03                       # Real time elapsed on the host
host_tick_rate                             1506958152                       # Simulator tick rate (ticks/s)
sim_freq                                 1000000000000                       # Frequency of simulated ticks
sim_insts                                        8489                       # Number of instructions simulated
sim_ops                                          9632                       # Number of ops (including micro ops) simulated
sim_seconds                                  0.000038                       # Number of seconds simulated
sim_ticks                                    37921000 
```

Εφόσον οι επεξεργαστές τρέχουν με τις ίδιες παραμέτρους (πχ. ρολόι επεξεργαστή και `sim_freq` ) μπορούμε να κάνουμε τις εξής συγκρίσεις:

Αρχικά από το `sim_seconds` καθώς και από το `final_tick` που στην περίπτωση του _MinorCPU_ είναι **33428000** ticks ενώ στον _TimingSimpleCPU_ είναι **37921000** , παρατηρούμε ότι ο πρώτος χρειάζεται λιγότερους κύκλους για την εκτέλεση του προγράμματος , κάτι που είναι λογικό εφόσον διαθέτει _pipelining_ .  

Επιπλέον το `host_seconds` το οποίο δηλώνει τα _πραγματικά_ δευτερόλεπτα που απαιτήθηκαν από το δικό μας σύστημα (_host_) για την κάθε προσομοίωση.Η διαφορά είναι μεγάλη μεταξύ των δύο μοντέλων ,**0.44** για τον **_Minor_** και **0.03** για τον **_TimingSimple_**. Το αποτέλεσμα αυτό είναι απόρροια της αρκετά περιπλοκότερης , λεπτομερέστερης και πιο ρεαλιστικής δομής του _Minor_ μοντέλου,όπως φαίνεται και στην παραπάνω περιγραφή.

Όσον αφορά τις ομοιότητες μεταξύ των 2 μοντέλων παρατηρήσαμε οτι τα `sim_insts ` και τα `sim_ops` παρέμειναν σχεδόν ίδια, εφόσον εκτελέσαμε το ίδιο πρόγραμμα.Ωστόσο η πολύ μικρή διαφορά που παρουσιάζεται οφείλεται στο γεγονός ότι ο **_Minor_** έχει πιο πολύπλοκη δομή και επομένως είναι λογικό να εκτελεί μερικές επιπλέον εντολές.

* **Υποερώτημα C**


Μας ζητήθηκε η αλλαγή κάποιων παραμέτρων του επεξεργαστή και η εκ νέου εκτέλεση του προγράμματος για τα δύο μοντέλα.Επιλέξαμε αρχικά να μεταβάλλουμε **μόνο** το **CPU Clock** απο τη _default_ τιμή των _2GHz_ σε **3.2Ghz**.Τις _default_ τιμές μπορεί κανείς να τις βρεί στο αρχείο **_configs/common/options.py_** και είναι _2GHz_ για το _clock_ και _DDR3_1600_8x8_ για τον τύπο μνήμης.Εκτελώντας την εντολή `./build/ARM/gem5.opt -d Minor-only-3.2   ./configs/example/se.py -c forloop --caches --cpu-type=MinorCPU  --cpu-clock="3.2GHz"` έχουμε

  - Αποτελέσματα απο το _stats.txt_ για **MinorCPU** με _clock_ **3.2GHz** και μνήμη **_DDR3_1600_8x8_**.

```
---------- Begin Simulation Statistics ----------
final_tick                                   30479314                       # Number of ticks from beginning of simulation 
host_inst_rate                                  23717                       # Simulator instruction rate (inst/s)
host_mem_usage                                 716568                       # Number of bytes of host memory used
host_op_rate                                    27015                       # Simulator op (including micro ops) rate (op/s)
host_seconds                                     0.36                       # Real time elapsed on the host
host_tick_rate                               84559867                       # Simulator tick rate (ticks/s)
sim_freq                                 1000000000000                       # Frequency of simulated ticks
sim_insts                                        8546                       # Number of instructions simulated
sim_ops                                          9737                       # Number of ops (including micro ops) simulated
sim_seconds                                  0.000030                       # Number of seconds simulated
sim_ticks    
```
Παρατηρήσαμε συγκρίνοντας τα στατιστικά του υποερωτήματος Β (που χρησιμοποιει  _default_ ρολόι 2GHz και  _DDR3_1600_8x8_ ) ότι η προσομοίωση γίνεται ταχύτερα με _clock_ **3.2GHz** .Παραθέτονται τα αντίστοιχα πεδία:

```
final_tick  33428000  # 2Ghz και DDR3_1600_8x8
final_tick  30479314  # 3.2Ghz και DDR3_1600_8x8 

sim_seconds 0.000033  # 2Ghz και DDR3_1600_8x8
sim_seconds 0.000030  # 3.2Ghz και DDR3_1600_8x8 
```
Συνεχίζοντας για τον _MinorCPU_ κρατάμε το _clock_ σταθερό στα **3.2GHz** .Αλλάζουμε τώρα τον τύπο μνήμης .Ανατρέχοντας στα διάφορα μοντέλα DRAM μνήμης που διαθέτει ο gem5 όπως αυτά αναφέρονται στο αρχείο **_src/mem/DRAMCtrl.py_** και στο [[3]](#DRAM),επιλέξαμε το **_DDR4_2400_16x4_** (2.4x16x(4/8)=19.2GBps).Με την εκτέλεση της εντολής `./build/ARM/gem5.opt -d minor-3.2-DDR4_2400_16X4   ./configs/example/se.py -c forloop --caches --cpu-type=MinorCPU  --cpu-clock="3.2GHz" --mem-type="DDR4_2400_16x4"
`

  - Αποτελέσματα απο το _stats.txt_ για **MinorCPU** με _clock_ **3.2GHz** και μνήμη **_DDR4_2400_16x4_**

```
---------- Begin Simulation Statistics ----------
final_tick                                   29717159                       # Number of ticks from beginning of simulation (restored from checkpoints and never reset)
host_inst_rate                                 162698                       # Simulator instruction rate (inst/s)
host_mem_usage                                 716568                       # Number of bytes of host memory used
host_op_rate                                   185086                       # Simulator op (including micro ops) rate (op/s)
host_seconds                                     0.05                       # Real time elapsed on the host
host_tick_rate                              564683227                       # Simulator tick rate (ticks/s)
sim_freq                                 1000000000000                       # Frequency of simulated ticks
sim_insts                                        8546                       # Number of instructions simulated
sim_ops                                          9737                       # Number of ops (including micro ops) simulated
sim_seconds                                  0.000030                       # Number of seconds simulated
sim_ticks                                    29717159                       # Number of ticks simulated
```
Συγκρίνουμε τώρα τον **MinorCPU** με _clock_ **3.2GHz** και μνήμη **DDR3_1600_8x8** με τον ίδιο αλλάζοντας τη μνήμη σε **_DDR4_2400_16x4_** . Βλέπουμε ότι η ταχύτητα αυξήθηκε ελάχιστα καθώς το `final_tick`  μειώθηκε λίγο .Η αλλαγή μνήμης θα είχε πιο αισθητά αποτελέσματα σε ένα πρόγραμμα που κάνει μεγαλύτερη χρήση της _DRAM_, σε αντίθεση με το απλοϊκό δικό μας. Παραθέτουμε τα αντίστοιχα πεδία:

```
final_tick  30479314  # 3.2Ghz και DDR3_1600_8x8 
final_tick  29717159  # 3.2Ghz και DDR4_2400_16x4

sim_seconds 0.000030  # 3.2Ghz και DDR3_1600_8x8 
sim_seconds 0.000030  # 3.2Ghz και DDR4_2400_16x4
```



Στη συνέχεια θα δούμε πως επηρεάζεται ο **TimingSimpleCPU** , αν μεταβάλλουμε τις ίδιες παραμέτρους και συγκρίνουμε τα αντίστοιχα στατιστικά.Πρώτα θα αλλάξουμε μόνο το ρολόι από την _default_ τιμή των **2Ghz** σε **3.2GHz** .Οι εντολές που εκτελέστηκαν είναι αντίστοιχες με τις παραπάνω με τη διαφορά στη παράμετρο `--cpu-type=TimingSimpleCPU`


  - Αποτελέσματα απο το _stats.txt_ για **TimingSimpleCPU** με _clock_ **3.2GHz** και μνήμη **_DDR3_1600_8x8_** :

```
---------- Begin Simulation Statistics ----------
final_tick                                   33410872                       # Number of ticks from beginning of simulation (restored from checkpoints and never reset)
host_inst_rate                                  13194                       # Simulator instruction rate (inst/s)
host_mem_usage                                 711956                       # Number of bytes of host memory used
host_op_rate                                    14969                       # Simulator op (including micro ops) rate (op/s)
host_seconds                                     0.64                       # Real time elapsed on the host
host_tick_rate                               51919970                       # Simulator tick rate (ticks/s)
sim_freq                                 1000000000000                       # Frequency of simulated ticks
sim_insts                                        8489                       # Number of instructions simulated
sim_ops                                          9632                       # Number of ops (including micro ops) simulated
sim_seconds                                  0.000033                       # Number of seconds simulated
sim_ticks                                    33410872                       # Number of ticks simulated
```
Σύγκριση του **TimingSimpleCPU** με _clock_ **3.2GHz** και μνήμη **_DDR3_1600_8x8_** με τον **ίδιο** αλλά με το _default_ clock των **2GHz**.

Παρατηρούμε πως το `final_tick` μειώθηκε , δηλαδή το πρόγραμμα εκτελέστηκε γρηγορότερα.

```
final_tick 37921000 # 2Ghz και DDR3_1600_8x8
final_tick 33410872 # 3.2Ghz και DDR3_1600_8x8 

sim_seconds 0.000038 # 2Ghz και DDR3_1600_8x8
sim_seconds 0.000033 # 3.2Ghz και DDR3_1600_8x8 
```
Τέλος διατηρώντας σταθερό το _clock_ των **3.2GHz** θα αντικαταστήσουμε τη μνήμη **_DDR3_1600_8x8_** με την **_DDR4_2400_16x4_** και θα ελέγξουμε αν υπάρχουν αλλαγές στους χρόνους εκτέλεσης.

  - Αποτελέσματα απο το _stats.txt_ για **TimingSimpleCPU** με _clock_ **3.2GHz** και μνήμη **_DDR4_2400_16x4_** :

```
---------- Begin Simulation Statistics ----------
final_tick                                   32911950                       # Number of ticks from beginning of simulation (restored from checkpoints and never reset)
host_inst_rate                                 174906                       # Simulator instruction rate (inst/s)
host_mem_usage                                 711956                       # Number of bytes of host memory used
host_op_rate                                   198047                       # Simulator op (including micro ops) rate (op/s)
host_seconds                                     0.05                       # Real time elapsed on the host
host_tick_rate                              676393983                       # Simulator tick rate (ticks/s)
sim_freq                                 1000000000000                       # Frequency of simulated ticks
sim_insts                                        8489                       # Number of instructions simulated
sim_ops                                          9632                       # Number of ops (including micro ops) simulated
sim_seconds                                  0.000033                       # Number of seconds simulated
sim_ticks                                    32911950                       # Number of ticks simulated
```
Σύγκριση του **TimingSimpleCPU** με _clock_ **3.2GHz** και μνήμη **_DDR3_1600_8x8_**  με τον **ίδιο** αλλάζοντας τη μνήμη σε **_DDR4_2400_16x4_** .

Όπως φαίνεται από το `final_tick` ο χρόνος εκτέλεσης του προγράμματος είναι ελάχιστα μικρότερος , που σημαίνει ότι η μνήμη δεν επηρέασε σε μεγάλο βαθμό τον  **TimingSimpleCPU** ως προς την ταχύτητα. Όπως προαναφέραμε και στην περίπτωση του **MinorCPU** η αλλαγή θα ήταν πιο αισθητή αν εκτελούσαμε ένα πιο περίπλοκο πρόγραμμα , το οποίο θα έκανε πιο ευρεία χρήση της _DRAM_ .Παραθέτουμε τα αντίστοιχα πεδία:

```
final_tick 33410872 # 3.2Ghz και DDR3_1600_8x8 
final_tick 32911950 # 3.2Ghz και DDR4_2400_16x4

sim_seconds 0.000033 # 3.2Ghz και DDR3_1600_8x8 
sim_seconds 0.000033 # 3.2Ghz και DDR4_2400_16x4
```


### Παράρτημα <a name="parartima"></a>

---
Εδώ θα παραθέσουμε μία σύντομη περιγραφή των  τεχνικών _memory access_ που αναφέρθηκαν πιο πάνω και χρησιμοποιούν τα  _in order_ μοντέλα επεξεργαστή _AtomicSimpleCPU_ , _TimingSimpleCPU_ και _MinorCPU_. Υπάρχει και η 3η τεχνική _functional access_ αλλά δε χρησιμοποιείται στην αναφορά μας.Περισσότερες πληροφορίες στη βιβλιογραφία.

* **Atomic Access**  
  Είναι η γρηγορότερη τεχνική εκ των τριών και λιγότερο λεπτομερής.Xρησιμοποιείται για _fast forwarding_  και για την προσεγγιστική χρονομέτρηση των requests στη μνήμη , αφού δε λαμβάνει υπόψη πολλές καθυστερήσεις(πχ _queuing delay_).Όταν στέλνεται στη μνήμη ένα _Atomic_ request , η μνήμη αποκρίνεται όταν κάνει _return_ και η συνάρτηση.

* **Timing Access**  
   Είναι η πιο λεπτομερής και πιο κοντά στη πραγματικότητα τεχνική προσπέλασης της μνήμης.Σε αυτή μοντελοποιούνται με όσο το δυνατόν βέλτιστο τρόπο οι καθυστερήσεις της μνήμης.Όταν στέλνεται ένα _Timing_ request στη μνήμη, αυτή θα αποκριθεί _κάποια_ στιγμη στο μέλλον.
  
  

  
### Βιβλιογραφία

---

<a name="#1"></a>
[1] https://github.com/arm-university/arm-gem5-rsk/blob/aa3b51b175a0f3b6e75c9c856092ae0c8f2a7cdc/gem5_rsk.pdf  
[2] http://learning.gem5.org/book/
<a name="DRAM"></a>  
[3] https://nitish2112.github.io/post/gem5-memory-model/  
[4] http://gem5.org/General_Memory_System#Atomic.2FTiming.2FFunctional_accesses


### Κριτκή Εργασίας
---

Η συγκεκριμένη εργασία είχε προφανώς σκοπό την εξοικείωση μας και μια πρώτη γνωριμία με τον _gem5_.Η εργασία κρίνεται απλοϊκή , αλλά αυτό μας βοήθησε να εξοικειωθούμε με το πρόγραμμα,να αναζητήσουμε πληροφορίες και να πειραματιστουμε χωρίς το βάρος εξεζητημένων απαιτήσεων που δε θα ταίριαζαν σε μία εισαγωγική εργασία,γι'αυτό πιστεύουμε πως ήταν απαραίτητη.Σημαντικό ήταν επίσης πως η εκφώνηση του πρώτου εργαστηρίου ήταν πολυ κατατοπιστική σε πολλά επίπεδα,όπως στα site που μπορούμε να ανατρέξουμε για πληροφορίες και tutorials,ξεκάθαρα ζητούμενα κλπ.Πιστεύουμε πως η εργασία πέτυχε το στόχο της , δηλαδή την ομαλή εισαγωγή στον _gem5_ και την κέντριση του ενδιαφέροντος μας για περαιτέρω αναζήτηση.  
Μια παρατήρηση που θα θέλαμε να επισημάνουμε είναι ότι στο α) υποερώτημα του 3ο ερωτήματος,δίνεται μια παραδειγματική εντολή εκτέλεσης χρησιμοποιόντας ως _config file_ το _configs/example/se.py_ και συγκεκριμένα αναφέρεται :  
   >(Παρατήρηση: Μην χρησιμοποιήσετε τα ίδιο configuration αρχείο (starter_se.py) αλλά το
αρχείο configs/example/se.py . Παράδειγμα εκτέλεσης:
$ ./build/ARM/gem5.opt configs/example/se.py --cpu=MinorCPU --caches tests/test-
progs/hello/bin/arm/linux/hello

**Ωστόσο στο δικό μας σύστημα** η εντολή αυτή δε λειτουργεί (Ubuntu 18.04) αλλά χρειάστηκε να την αντικαταστήσουμε με την `./build/ARM/gem5.opt ./configs/example/se.py -c tests/test-progs/hello/bin/arm/linux/hello --caches --cpu-type=MinorCPU`

Όλα τα άλλα μας καθοδήγησαν και λειτούργησαν άψογα.
