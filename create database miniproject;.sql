create database miniproject;

create table personas (id int primary key, phone varchar(25), address varchar(50), fname varchar(25), lname varchar(25), dna varchar(32));
                        
create table investigator (id int primary key, department varchar(25), rank int check (rank > 0 and rank < 5), cases_worked int not null check (cases_worked >= 0), dna varchar(32) not null,           -- cases is a derived attribute, add this maybe
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


create or replace function number_of_cases() return trigger as $$
-- declare the function:
begin
update investigator set cases_worked = (
  select count(*) 
  from works
  where works.investigator_id = investigator.id);
  return null;
  end;
  $$ language plpgsql;
  
create trigger calc_cases_worked
after update or delete or insert on works
for each row execute function number_of_cases();
  
create table load_data(
  id int primary key, 
  first_name varchar(25),
  last_name varchar(50),
  address varchar(50),
  dna varchar(32),
  phone_number varchar(50),
  role int,
  rank int,
  department varchar(25),
  cases_solved int,
  height int,
  hair_color varchar(25),
  eye_color varchar(25));
      
      
  -- copy commands are executed one by one, path to file has to manually configured
-- this one looks complicated because information from one table has to be split 
\copy load_data from 'insert path to file here'
delimiter ';' csv header;
insert into personas(
  select t.id, t.phone_number, t.address, t.first_name, t.last_name, t.dna from load_data t);
  where d.role = 1);
  
insert into investigator(
  select d.id, d.department, d.rank, 0, t.dna from load_data d
  where d.role=2);
  
insert into suspect(
  select d.id, d.height, d.hair_color, d.eye_color from load_data d
  where d.role = 2 or d.role = 3);
  
drop table load_data;

-- todo: reorganize the original tables to fit our database,
--        check the delimiters 
-- check if the right attributes are being imported

\copy crimecase from 'insert path here' delimiter ',' csv header;

\copy evidence from 'insert path to file here' delimiter ',' csv header;

\copy works from 'insert path to file here' delimiter ',' csv header;

\copy suspected from 'insert path to file here' delimiter ',' csv header;
  
\copy interrogation from 'insert path to file here' delimiter ',' csv header;
    

-- sudo su - postgres -c 'psql'
