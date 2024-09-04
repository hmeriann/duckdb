SELECT 
  list_aggregate(tau_pt, 'first'),
  list_aggregate(tau_eta, 'first'),
  list_aggregate(jet_pt, 'first'),
  list_aggregate(jet_eta, 'first'),
  list_aggregate(muon_pt, 'first'),
  list_aggregate(muon_eta, 'first'),
  list_aggregate(el_pt, 'first'),
  list_aggregate(el_eta, 'first'),
  list_aggregate(ph_pt, 'first'),
  list_aggregate(ph_eta, 'first')  
FROM singleMu;