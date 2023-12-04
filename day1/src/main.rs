use regex::Regex;
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
    let input_file = File::open(args[1].clone())?;
    let buffered_reader = io::BufReader::new(input_file);

    let mut lines_vector: Vec<String> = Vec::new();
    for line in buffered_reader.lines() {
        let line_content = line?;
        lines_vector.push(line_content);
    }

    let digit_map = create_digit_map();
    let digit_regex = Regex::new(r"(one|two|three|four|five|six|seven|eight|nine|\d)").unwrap();
    let mut two_digits = Vec::new();

    for line in &lines_vector {
        let first_digit = find_first_digit(line, &digit_map, &digit_regex);
        let last_digit = find_last_digit(line, &digit_map, &digit_regex);


        if let (Some(first), Some(last)) = (first_digit, last_digit) {
            let combined_digits = format!("{}{}", first, last);
            two_digits.push(combined_digits.parse::<i32>().unwrap());
        }
    }

    let sum: i32 = two_digits.iter().sum();
    println!("Sum: {:?}", sum);
    Ok(())
}
fn create_digit_map() -> HashMap<String, i32> {
    let mut map = HashMap::new();
    map.insert("one".to_string(), 1);
    map.insert("two".to_string(), 2);
    map.insert("three".to_string(), 3);
    map.insert("four".to_string(), 4);
    map.insert("five".to_string(), 5);
    map.insert("six".to_string(), 6);
    map.insert("seven".to_string(), 7);
    map.insert("eight".to_string(), 8);
    map.insert("nine".to_string(), 9);
    map
}
fn find_first_digit(
    line: &str,
    digit_map: &HashMap<String, i32>,
    digit_regex: &Regex,
) -> Option<i32> {
    digit_regex
        .find(line)
        .and_then(|m| match m.as_str().parse::<i32>() {
            Ok(num) => Some(num),
            Err(_) => digit_map.get(m.as_str()).copied(),
        })
}

fn find_last_digit(
    line: &str,
    digit_map: &HashMap<String, i32>,
    digit_regex: &Regex,
) -> Option<i32> {
    let mut last_match = None;
    let mut start = 0;

    while let Some(match_item) = digit_regex.find(&line[start..]) {
        let global_start = match_item.start() + start;
        let global_end = match_item.end() + start;
        let match_str = &line[global_start..global_end];

       

        last_match = Some(match_str.to_string());
        start = global_start + 1;

        if start >= line.len() {
            break;
        }
    }

    last_match.and_then(|m| {
        match m.parse::<i32>() {
            Ok(num) => {
                Some(num)
            }
            Err(_) => {
                let digit = digit_map.get(&m).copied();
                digit
            }
        }
    })
}
