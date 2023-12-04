use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead};
use std::env;

fn main() -> io::Result<()> {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        eprintln!("Please provide the input file");
        return Ok(());
    }
    let input_file = File::open(&args[1])?;
    let buffered_reader = io::BufReader::new(input_file);

    let mut sum = 0;

    for line in buffered_reader.lines() {
        let line_content = line?;
        let parts: Vec<&str> = line_content.split(": ").collect();
        let sets: Vec<&str> = parts[1].split("; ").collect();

        let mut min_cubes = HashMap::new();
        min_cubes.insert("red", 0);
        min_cubes.insert("green", 0);
        min_cubes.insert("blue", 0);

        for set in sets {
            let cubes: Vec<&str> = set.split(", ").collect();
            let mut set_cubes = HashMap::new();
            
            for cube in cubes {
                let cube_parts: Vec<&str> = cube.split(" ").collect();
                let count: i32 = cube_parts[0].parse().unwrap();
                let color = cube_parts[1];
                set_cubes.entry(color).and_modify(|e| *e = i32::max(*e, count)).or_insert(count);
            }

            for (color, count) in set_cubes {
                *min_cubes.entry(color).or_insert(0) = i32::max(*min_cubes.get(color).unwrap(), count);
            }
        }

        let power = min_cubes.values().product::<i32>();
        sum += power;
    }

    println!("{}", sum);
    Ok(())
}
