create database miniproject;

create table personas (id int primary key, phone varchar(25), address varchar(50), fname varchar(25), lname varchar(25), dna varchar(32));
                        
create table investigator (id int primary key, department varchar(25), rank int check (rank > 0 and rank < 5),               -- cases is a derived attribute, add this maybe
                        foreign key (id) references personas);                                                                      -- must have hash dna

create table suspect (id int primary key, height int, haircolor varchar(25), eyecolor varchar(25),          
                        foreign key (id) references personas);

create table crimecase (id int primary key, place varchar(50), date varchar(25),
                        time varchar(25), type varchar(25), status varchar(25) check (status = 'open' or status = 'closed'),  investigator_id int not NULL,
                        foreign key (investigator_id) references investigator);

create table evidence (storage_id int primary key, description varchar(200), characteristic varchar(100), tag varchar(25), persona_id int, case_id int,
                        foreign key(persona_id) references personas, 
                        foreign key(case_id) references crimecase);

create table works (case_id int , investigator_id int, startdate varchar(25), stopdate varchar(25),
                        primary key(case_id, investigator_id),
                        foreign key(case_id)references crimecase, 
                        foreign key(investigator_id) references investigator);

create table suspected (case_id int, suspect_id int, status varchar(25) check(status = 'active' or status = 'inactive'),        
                        primary key(case_id, suspect_id),
                        foreign key(case_id) references crimecase, 
                        foreign key(suspect_id) references suspect);

create table interrogation (suspect_id int, investigator_id int, date varchar(25), time varchar(25), case_id int,               -- do we have a composite key from all attributes????????????????????
                            primary key(suspect_id, investigator_id, date, time, case_id),
                            foreign key(suspect_id) references suspect, 
                            foreign key(investigator_id) references investigator, foreign key(case_id) references crimecase);   --primary key date and time???

create view investigator_cases as 
                            select count(*) investigator_id 
                            from works group by investigator_id;

alter table personas add constraint dna 
    check (dna != null or -- cannot use subquery in check constraint
    exists(select p.id from personas p, suspect s where p.id = s.id)); 


-- sudo su - postgres -c 'psql'